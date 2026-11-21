#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u8 *buf;
} UnkObj_02068478;

extern void *gMainHeap;

u8 *ObjData_Load(UnkObj_02068478 *s, s32 a, s32 b);
void NNS_G3dResDefaultSetup(void *p);
u32 Asset3D_GetResourceSize(void *p);
void Asset3D_CopyResource(void *p, void *buf);

static inline u8 *Alloc_02068478(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
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

static inline void Free_02068478(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void ObjData_SetupModelResource(UnkObj_02068478 *s, s32 a) {
    u8 *res;

    res = s->buf;
    if (res == NULL)
        res = ObjData_Load(s, a, 0);
    NNS_G3dResDefaultSetup(res);
    s->buf = Alloc_02068478((Asset3D_GetResourceSize(res) + 0x43) & ~0x1f);
    Asset3D_CopyResource(res, s->buf);
    Free_02068478(res);
}
