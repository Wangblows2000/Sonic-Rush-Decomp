#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;
    u8 pad24[4];
    u32 pos;
    u8 pad2c[0x18];
} UnkFSFile_0203bdcc;

typedef struct {
    u32 ofs;
    u32 size;
} UnkRange_0203bdcc;

typedef struct {
    u8 pad0[0x40];
    UnkRange_0203bdcc ovt9;
    UnkRange_0203bdcc fat;
    UnkRange_0203bdcc ovt9b;
    UnkRange_0203bdcc ovt7;
} UnkRomHeader_0203bdcc;

typedef struct {
    u8 pad0[0x18];
    u32 fileId;
    u8 pad1c[4];
} UnkOvl_0203bdcc;

typedef struct {
    u8 pad0[0x14];
    void *(*alloc)(void *heap, u32 size, void *ptr);
    void *heap;
    u8 pad1c[4];
    u32 *table;
    u32 tableSize;
} UnkCtx_0203bdcc;

extern UnkCtx_0203bdcc data_022b8be0;
extern char data_02078068[];
void func_0200be70(UnkFSFile_0203bdcc *f);
void *func_0200b828(const char *name, u32 len);
BOOL func_0200bbec(UnkFSFile_0203bdcc *f, void *arc, u32 top, u32 bottom, s32 id);
s32 func_0200b99c(void *f, void *buf, u32 size);
BOOL func_0200b930(void *f, u32 pos, u32 whence);
void func_0200bad8(UnkFSFile_0203bdcc *f);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);

void func_0203bdcc(UnkFSFile_0203bdcc *src, BOOL arg) {
    UnkRange_0203bdcc tab[4];
    u32 sizes[4];
    u32 *ptrs[4];
    UnkRomHeader_0203bdcc hdrBuf;
    UnkFSFile_0203bdcc file;
    u32 *buf;
    BOOL single;
    u32 total;
    s32 i;
    u32 e;
    u32 fatOfs;
    u32 base0;
    BOOL merge;
    u32 base;
    UnkRomHeader_0203bdcc *hdr;
    u32 *p;
    s32 k;

    total = 0;
    single = (src == NULL);
    merge = FALSE;
    if (!single && arg)
        merge = TRUE;
    func_0200be70(&file);
    func_0200bbec(&file, func_0200b828(data_02078068, 3), 0, 0x7fffffff, -1);
    if (single) {
        base = 0;
        base0 = base;
        hdr = (UnkRomHeader_0203bdcc *)0x027ffe00;
    } else {
        u32 delta;
        hdr = &hdrBuf;
        delta = src->pos - src->top;
        base = delta + src->top;
        base0 = base;
        func_0200b99c(src, hdr, 0x60);
        func_0200b930(src, delta, 0);
    }
    if (!merge) {
        tab[0] = hdr->fat;
        tab[1] = hdr->ovt9;
        tab[2] = hdr->ovt9b;
        tab[3] = hdr->ovt7;
    } else {
        base0 = 0;
        tab[0] = *(UnkRange_0203bdcc *)0x027ffe48;
        tab[1] = *(UnkRange_0203bdcc *)0x027ffe40;
        tab[2].ofs = hdr->ovt9b.ofs + base;
        tab[2].size = hdr->ovt9b.size;
        tab[3].ofs = hdr->ovt7.ofs + base;
        tab[3].size = hdr->ovt7.size;
    }
    total += 4;
    for (i = 0; i < 4; i++) {
        sizes[i] = (tab[i].size + 0x1f) & ~0x1f;
        total += sizes[i] + 4;
    }
    if (merge) {
        for (i = 2; i < 4; i++) {
            u32 x = (((tab[i].size >> 5) << 3) + 0x1f) & ~0x1f;
            sizes[0] += x;
            total += x;
        }
    }
    e = OS_DisableInterrupts();
    buf = data_022b8be0.alloc(data_022b8be0.heap, total, NULL);
    OS_RestoreInterrupts(e);
    *buf = base0;
    p = buf + 1;
    for (i = 0; i < 4; i++) {
        u32 *q;
        ptrs[i] = p;
        q = ptrs[i];
        *q = tab[i].size;
        func_0200b930(&file, base0 + tab[i].ofs, 0);
        func_0200b99c(&file, q + 1, *q);
        p = (u32 *)((u8 *)p + (sizes[i] + 4));
    }
    if (merge) {
        UnkRange_0203bdcc *ent;
        u32 n;
        ent = (UnkRange_0203bdcc *)(ptrs[0] + 1);
        n = *ptrs[0] >> 3;
        fatOfs = base + hdr->fat.ofs;
        for (k = 2; k < 4; k++) {
            u32 *ov;
            UnkOvl_0203bdcc *ovl;
            s32 j;
            s32 cnt;
            ov = ptrs[k];
            ovl = (UnkOvl_0203bdcc *)(ov + 1);
            cnt = *ov >> 5;
            for (j = 0; j < cnt; j++) {
                func_0200b930(&file, fatOfs + ovl[j].fileId * 8, 0);
                func_0200b99c(&file, &ent[n], 8);
                ent[n].ofs += base;
                ent[n].size += base;
                ovl[j].fileId = n;
                n++;
            }
        }
        *ptrs[0] = n << 3;
    }
    DC_FlushRange(buf, total);
    DC_WaitWriteBufferEmpty();
    func_0200bad8(&file);
    data_022b8be0.table = buf;
    data_022b8be0.tableSize = total;
}
