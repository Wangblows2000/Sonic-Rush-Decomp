#include <nitro/types.h>

typedef struct { u32 off; u32 size; } UnkEnt_02060fd8;
typedef struct { u8 data[0x48]; } UnkFile_02060fd8;
typedef struct { u8 pad0[4]; u32 flags; u8 pad8[0x68]; s32 unk70; } UnkWork_02060fd8;
typedef struct { u16 base; s16 cur; } UnkCtr_02060fd8;

extern const char *data_0207cbdc[];
extern void *gMainHeap;

void FS_InitFile(UnkFile_02060fd8 *f);
BOOL FS_OpenFile(UnkFile_02060fd8 *f, const char *path);
BOOL FS_SeekFile(UnkFile_02060fd8 *f, s32 ofs, s32 whence);
s32 FS_ReadFile(UnkFile_02060fd8 *f, void *dst, s32 len);
BOOL FS_CloseFile(UnkFile_02060fd8 *f);
void DC_StoreRange(void *p, u32 size);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline u8 *Alloc_02060fd8(u32 size) {
    u8 *p = Heap_Alloc(gMainHeap, size, -0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

u8 *TalkDemo_ReadMessage(UnkWork_02060fd8 *w, UnkCtr_02060fd8 *c) {
    UnkEnt_02060fd8 ent = {0, 0};
    const char *path = data_0207cbdc[w->unk70];
    s32 n;
    u8 *p;
    UnkFile_02060fd8 f;

    if (w->flags & 0x20)
        return NULL;
    n = c->base + c->cur++;
    FS_InitFile(&f);
    FS_OpenFile(&f, path);
    FS_SeekFile(&f, n * 8 + 8, 0);
    FS_ReadFile(&f, &ent, 8);
    DC_StoreRange(&ent, 8);
    p = Alloc_02060fd8((ent.size + 0x43) & ~0x1f);
    FS_SeekFile(&f, ent.off, 0);
    FS_ReadFile(&f, p, ent.size);
    DC_StoreRange(p, ent.size);
    FS_CloseFile(&f);
    return p;
}
#pragma thumb reset
