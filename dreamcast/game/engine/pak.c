/* /cd/data.pak reader (pak.h; format: tools/dreamcast/mkpak.py) */
#include "pak.h"
#include "loading.h"
#include "memtrack.h"
#include <kos.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define PAK_PATH "/cd/data.pak"
typedef struct { uint32_t hash, offset, size; } PakEntry;
static int s_state;              /* 0 not tried, 1 open, -1 absent */
static file_t s_file = FILEHND_INVALID;
static PakEntry *s_index;
static uint32_t s_count, s_ndirs;
static char (*s_dirs)[64];
static uint32_t s_files, s_bytes;

/* Read-ahead: one GD-ROM request costs milliseconds whatever its size, and the pack keeps
 * a resource's files together in load order, so a miss reads PAK_RA bytes from the file's
 * offset and the next small files of the same resource come from RAM. */
#define PAK_RA (64 * 1024)
static uint8_t *s_ra;
static uint32_t s_ra_off, s_ra_len, s_reads;

static int pak_read(uint32_t off, void *dst, uint32_t n)
{
    if(s_ra && off >= s_ra_off && off + n <= s_ra_off + s_ra_len) {
        memcpy(dst, s_ra + (off - s_ra_off), n);
        return 0;
    }
    if(fs_seek(s_file, off, SEEK_SET) != (off_t)off) return -1;
    s_reads++;
    if(n >= PAK_RA / 2 || (!s_ra && !(s_ra = memalign(32, PAK_RA))))
        return loading_read(s_file, dst, n) == (ssize_t)n ? 0 : -1;   /* big file: straight in */
    s_ra_len = 0;
    {
        ssize_t r = loading_read(s_file, s_ra, PAK_RA);   /* ends early at the end of the pack */
        if(r < (ssize_t)n) return -1;
        s_ra_off = off;
        s_ra_len = (uint32_t)r;
    }
    memcpy(dst, s_ra, n);
    return 0;
}

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 0x811c9dc5u;
    while(*s) h = (h ^ (uint8_t)*s++) * 0x01000193u;
    return h;
}

static int pak_init(void)
{
    uint32_t hdr[4];
    if(s_state) return s_state > 0;
    s_state = -1;
    s_file = fs_open(PAK_PATH, O_RDONLY);
    if(s_file == FILEHND_INVALID) return 0;
    if(fs_read(s_file, hdr, 16) != 16 || memcmp(hdr, "SPAK", 4) || hdr[1] != 1 || hdr[3] > 64) goto bad;
    s_count = hdr[2];
    s_ndirs = hdr[3];
    s_dirs = malloc(64 * s_ndirs + 1);
    s_index = malloc(sizeof(PakEntry) * s_count + 1);
    if(!s_dirs || !s_index) goto bad;
    if(fs_read(s_file, s_dirs, 64 * s_ndirs) != (ssize_t)(64 * s_ndirs) ||
       loading_read(s_file, s_index, sizeof(PakEntry) * s_count) != (ssize_t)(sizeof(PakEntry) * s_count))
        goto bad;
    s_state = 1;
    return 1;
bad:
    free(s_dirs); free(s_index); s_dirs = NULL; s_index = NULL;
    fs_close(s_file); s_file = FILEHND_INVALID;
    return 0;
}

/* stage pack: read whole at pak_stage_begin, served from RAM until pak_stage_end */
static uint8_t *s_stage;
static uint32_t s_stage_size;

static const PakEntry *find(const PakEntry *ix, uint32_t n, uint32_t h)
{
    uint32_t lo = 0, hi = n;
    while(lo < hi) {   /* index sorted by hash */
        uint32_t m = (lo + hi) / 2;
        if(ix[m].hash < h) lo = m + 1; else hi = m;
    }
    return lo < n && ix[lo].hash == h ? &ix[lo] : NULL;
}

int pak_stage_begin(const char *path)
{
    file_t f;
    uint32_t n;
    pak_stage_end();
    f = fs_open(path, O_RDONLY);
    if(f == FILEHND_INVALID) return -1;
    n = fs_total(f);
    s_stage = memalign(32, n + 4);
    if(s_stage && loading_read(f, s_stage, n) == (ssize_t)n && n >= 16 && !memcmp(s_stage, "SPAK", 4) &&
       16 + 64 * ((uint32_t *)s_stage)[3] + 12 * ((uint32_t *)s_stage)[2] <= n) {
        s_stage_size = n;
        s_reads++;
    } else {
        free(s_stage);
        s_stage = NULL;
    }
    fs_close(f);
    return s_stage ? 0 : -1;
}

void pak_stage_end(void)
{
    free(s_stage);
    s_stage = NULL;
    s_stage_size = 0;
}

static int stage_load(const char *key, void **out, uint32_t *size)
{
    const uint32_t *h32 = (const uint32_t *)s_stage;
    const PakEntry *e = find((const PakEntry *)(s_stage + 16 + 64 * h32[3]), h32[2], fnv1a(key));
    if(!e || e->offset + e->size > s_stage_size) return 0;
    *out = memalign(32, e->size + 32);
    if(!*out) return 0;
    memcpy(*out, s_stage + e->offset, e->size);
    memset((uint8_t *)*out + e->size, 0, 32);
    if(size) *size = e->size;
    s_files++;
    s_bytes += e->size;
    return 1;
}

static int pak_load_(const char *path, void **out, uint32_t *size);
int pak_load(const char *path, void **out, uint32_t *size)
{
    int r = pak_load_(path, out, size);
    if(r == PAK_FOUND && memtrack_retag)   /* malloc-tracking builds: the block is the caller's */
        memtrack_retag(*out, __builtin_return_address(0));
    return r;
}
/* files read ahead in a load (pak_prefetch) for a pak_load later in the scene */
#define PF_MAX 6
static struct { char path[64]; void *buf; uint32_t size; uint32_t used; } s_pf[PF_MAX];
static uint32_t s_pf_clock;

int pak_prefetch(const char *path)
{
    void *b;
    uint32_t sz = 0;
    int i, k = -1;
    if(strlen(path) >= sizeof(s_pf[0].path)) return -1;
    for(i = 0; i < PF_MAX; i++)
        if(s_pf[i].buf && !strcmp(s_pf[i].path, path)) { s_pf[i].used = ++s_pf_clock; return 0; }
    if(pak_load_(path, &b, &sz) != PAK_FOUND) return -1;
    for(i = 0; i < PF_MAX; i++)   /* a free slot, else the least recently prefetched one */
        if(k < 0 || !s_pf[i].buf || (s_pf[k].buf && s_pf[i].used < s_pf[k].used)) k = i;
    free(s_pf[k].buf);
    strcpy(s_pf[k].path, path);
    s_pf[k].buf = b;
    s_pf[k].size = sz;
    s_pf[k].used = ++s_pf_clock;
    return 0;
}

void pak_prefetch_drop(void)
{
    int i;
    for(i = 0; i < PF_MAX; i++) { free(s_pf[i].buf); s_pf[i].buf = NULL; }
}

static int pak_load_(const char *path, void **out, uint32_t *size)
{
    const char *key;
    uint32_t h, lo = 0, hi, i;
    int covered = 0;
    *out = NULL;
    if(strncmp(path, "/cd/", 4) != 0) return PAK_NONE;
    for(i = 0; i < PF_MAX; i++)   /* read ahead: the buffer is the caller's now */
        if(s_pf[i].buf && !strcmp(s_pf[i].path, path)) {
            *out = s_pf[i].buf;
            if(size) *size = s_pf[i].size;
            s_pf[i].buf = NULL;
            return PAK_FOUND;
        }
    if(s_stage && stage_load(path + 4, out, size)) return PAK_FOUND;
    if(!pak_init()) return PAK_NONE;
    key = path + 4;
    for(i = 0; i < s_ndirs && !covered; i++)
        covered = strncmp(key, s_dirs[i], strnlen(s_dirs[i], 64)) == 0;
    if(!covered) return PAK_NONE;
    h = fnv1a(key);
    hi = s_count;
    while(lo < hi) {   /* index sorted by hash */
        uint32_t m = (lo + hi) / 2;
        if(s_index[m].hash < h) lo = m + 1; else hi = m;
    }
    if(lo >= s_count || s_index[lo].hash != h) return PAK_MISSING;
    *out = memalign(32, s_index[lo].size + 32);   /* + 32 zero bytes, as the module loaders pad */
    if(!*out) return PAK_MISSING;
    memset((uint8_t *)*out + s_index[lo].size, 0, 32);
    if(pak_read(s_index[lo].offset, *out, s_index[lo].size) != 0) {
        free(*out);
        *out = NULL;
        return PAK_MISSING;
    }
    if(size) *size = s_index[lo].size;
    s_files++;
    s_bytes += s_index[lo].size;
    return PAK_FOUND;
}

void pak_stats(uint32_t *files, uint32_t *bytes, uint32_t *reads)
{
    if(files) *files = s_files;
    if(bytes) *bytes = s_bytes;
    if(reads) *reads = s_reads;
}
