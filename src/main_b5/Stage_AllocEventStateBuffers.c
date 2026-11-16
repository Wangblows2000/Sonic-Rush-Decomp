#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 *buf0;
    u32 size0;
    u8 *buf1;
    u32 size1;
} UnkStruct_022c4520;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4520 data_022c4520;
extern void *data_ov000_0236aa8c;
extern u8 data_0207a098;
extern void *gSystemHeap;

void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void NNS_FndMountArchive(void *f, void *path, void *arc);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline u8 *Alloc_0204cc48(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gSystemHeap, size, -0x20);
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

void Stage_AllocEventStateBuffers(void) {
    UnkStruct_022c4520 *s;
    u32 n;
    u32 f[0x18];

    if (gGameState.flags & 0x40)
        return;
    s = &data_022c4520;
    NNS_FndMountArchive(f, &data_0207a098, data_ov000_0236aa8c);
    n = *(u32 *)NNS_FndGetArchiveFileByIndex(f, 1) >> 8;
    s->size0 = n;
    s->buf0 = Alloc_0204cc48((n + 0x43) & ~0x1f);
    s->size1 = *(u32 *)NNS_FndGetArchiveFileByIndex(f, 2) >> 8;
    s->buf1 = Alloc_0204cc48((s->size1 + 0x43) & ~0x1f);
    NNS_FndUnmountArchive(f);
}
