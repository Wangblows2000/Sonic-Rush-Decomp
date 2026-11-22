/*
 * Host backend for the sound runtime tests: stdio file access, the wave blob
 * kept in RAM, AICA commands recorded instead of sent.
 */
#include "snd_internal.h"
#include "snd_out_host.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HostOutLog g_host_out;

void snd_out_init(void) { memset(&g_host_out, 0, sizeof g_host_out); }
void snd_out_shutdown(void) {}

int snd_out_upload(SndBank *b, const void *blob, uint32_t size) {
    b->host_blob = malloc(size ? size : 1);
    if (!b->host_blob) return -1;
    memcpy(b->host_blob, blob, size);
    b->aram = 0x30000; /* AICA_RAM_START: any non-zero base */
    g_host_out.uploaded += size;
    return 0;
}
void snd_out_release(SndBank *b) { free(b->host_blob); b->host_blob = NULL; b->aram = 0; }

static void log_ev(int type, int ch, uint32_t a, uint32_t freq, int vol, int pan, unsigned what) {
    HostOutEvent *e;
    if (g_host_out.n >= HOST_OUT_MAX) { g_host_out.dropped++; return; }
    e = &g_host_out.ev[g_host_out.n++];
    e->type = type; e->ch = ch; e->aram = a; e->freq = freq; e->vol = vol; e->pan = pan; e->what = what;
}
void snd_out_start(int ch, uint32_t aram, int fmt, uint32_t samples, int loop, uint32_t loop_start,
                   uint32_t freq, int vol, int pan) {
    (void)fmt; (void)samples; (void)loop; (void)loop_start;
    g_host_out.starts++;
    g_host_out.on[ch] = 1;
    log_ev(1, ch, aram, freq, vol, pan, 0);
}
void snd_out_update(int ch, unsigned what, uint32_t freq, int vol, int pan) {
    g_host_out.updates++;
    log_ev(2, ch, 0, freq, vol, pan, what);
}
void snd_out_stop(int ch) {
    g_host_out.stops++;
    g_host_out.on[ch] = 0;
    log_ev(3, ch, 0, 0, 0, 0, 0);
}

void *snd_out_read_file(const char *path, uint32_t off, uint32_t size) {
    FILE *f = fopen(path, "rb");
    void *buf;
    if (!f) return NULL;
    buf = malloc(size ? size : 1);
    if (!buf || fseek(f, (long)off, SEEK_SET) || fread(buf, 1, size, f) != size) { free(buf); fclose(f); return NULL; }
    fclose(f);
    return buf;
}
int snd_out_file_size(const char *path) {
    FILE *f = fopen(path, "rb");
    long n;
    if (!f) return -1;
    fseek(f, 0, SEEK_END); n = ftell(f); fclose(f);
    return (int)n;
}
