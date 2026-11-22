/*
 * sp_data.c - loader of build/dreamcast/special/spdata.bin (tools/spdata.py):
 * the overlay-1 / arm9 tables the special-stage code reads, by DS address, and
 * the address -> function map for the code pointers inside those tables.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

char g_sp_path[128] = "/cd/special";
int g_sp_language = 1;

typedef struct {
    char name[24];
    u32 addr, ofs, size;
} SpBlobEnt;

static u8 *s_data;
static u32 s_size;
static const SpBlobEnt *s_blobs;
static u32 s_nblobs;

void special_set_path(const char *dir) {
    strncpy(g_sp_path, dir, sizeof(g_sp_path) - 1);
    g_sp_path[sizeof(g_sp_path) - 1] = 0;
}
void special_set_language(int lang) { g_sp_language = lang; }

void *sp_file_load(const char *rel, u32 *size) {
    char path[256];
    FILE *f;
    long n;
    u8 *b;
    snprintf(path, sizeof(path), "%s/%s", g_sp_path, rel);
    f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = (u8 *)malloc((size_t)n + 32);
    if (b && fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        b = NULL;
    }
    fclose(f);
    if (b && size)
        *size = (u32)n;
    return b;
}

int sp_data_load(void) {
    if (s_data)
        return 0;
    s_data = (u8 *)sp_file_load("spdata.bin", &s_size);
    if (!s_data || s_size < 16 || memcmp(s_data, "SPD1", 4)) {
        free(s_data);
        s_data = NULL;
        return -1;
    }
    memcpy(&s_nblobs, s_data + 8, 4);
    {
        u32 ofs;
        memcpy(&ofs, s_data + 12, 4);
        s_blobs = (const SpBlobEnt *)(s_data + ofs);
    }
    g_sp_sincos = (const s16 *)sp_rom(0x0206e470, 0x4000);
    g_sp_atan = (const s16 *)sp_rom(0x02072470, 0x102);
    return (g_sp_sincos && g_sp_atan) ? 0 : -1;
}

void sp_data_free(void) {
    free(s_data);
    s_data = NULL;
    s_blobs = NULL;
    s_nblobs = 0;
}

const void *sp_rom(u32 addr, u32 size) {
    u32 i;
    for (i = 0; i < s_nblobs; i++) {
        const SpBlobEnt *b = &s_blobs[i];
        if ((!strcmp(b->name, "ov1") || !strcmp(b->name, "arm9")) && addr >= b->addr &&
            addr + size <= b->addr + b->size)
            return s_data + b->ofs + (addr - b->addr);
    }
    fprintf(stderr, "special: ROM table 0x%08x (+%u) not in spdata.bin\n", (unsigned)addr, (unsigned)size);
    abort();
    return NULL;
}

const void *sp_blob(const char *name, u32 *size) {
    u32 i;
    for (i = 0; i < s_nblobs; i++)
        if (!strcmp(s_blobs[i].name, name)) {
            if (size)
                *size = s_blobs[i].size;
            return s_data + s_blobs[i].ofs;
        }
    return NULL;
}

/* native implementations of the ROM functions that the tables point to */
typedef struct {
    const char *name;
    SpFn fn;
} SpFuncEnt;
extern const SpFuncEnt g_sp_func_table[];

const char *sp_func_name(u32 ds_addr) {
    u32 n, i;
    const u8 *f;
    if (ds_addr == 0)
        return NULL;
    f = (const u8 *)sp_blob("funcs", &n);
    for (i = 0; f && i < n / 64; i++) {
        u32 a;
        memcpy(&a, f + i * 64, 4);
        if (a == ds_addr)
            return (const char *)(f + i * 64 + 4);
    }
    return NULL;
}

SpFn sp_func(u32 ds_addr) {
    const char *nm;
    int i;
    if (ds_addr == 0)
        return NULL;
    nm = sp_func_name(ds_addr);
    if (!nm) {
        fprintf(stderr, "special: no symbol for code pointer 0x%08x\n", (unsigned)ds_addr);
        abort();
    }
    for (i = 0; g_sp_func_table[i].name; i++)
        if (!strcmp(g_sp_func_table[i].name, nm))
            return g_sp_func_table[i].fn;
    fprintf(stderr, "special: function %s (0x%08x) not translated\n", nm, (unsigned)ds_addr);
    abort();
    return NULL;
}
