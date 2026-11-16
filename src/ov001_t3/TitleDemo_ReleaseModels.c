#include <nitro/types.h>

typedef struct {
    u8 pad[0x60];
} UnkArc_022d6738;

typedef struct {
    u8 pad0[0x14];
    u8 obj14[0x14c - 0x14];
    u8 obj14c[0x294 - 0x14c];
    void *res0;
    void *res1;
} UnkStruct_022d6738;

extern void *gMainHeap;
extern char data_ov001_0231b7f8[];

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *o);
BOOL func_02021310(UnkArc_022d6738 *arc, const char *name, void *narc);
void *func_02021264(UnkArc_022d6738 *arc, u32 idx);
void func_0202566c(void *res);
BOOL func_020212e0(UnkArc_022d6738 *arc);

#pragma thumb on
static inline void Free_022d6738(void *p) {
    u32 size;
    if (OS_GetArenaLo(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void TitleDemo_ReleaseModels(UnkStruct_022d6738 *p) {
    UnkArc_022d6738 arc;

    if (p->res0 == NULL) return;
    Model_Release(p->obj14);
    Model_Release(p->obj14c);
    func_02021310(&arc, data_ov001_0231b7f8, p->res0);
    func_0202566c(func_02021264(&arc, 0));
    func_0202566c(func_02021264(&arc, 3));
    func_020212e0(&arc);
    Free_022d6738(p->res0);
    p->res0 = NULL;
    Free_022d6738(p->res1);
    p->res1 = NULL;
}
#pragma thumb reset
