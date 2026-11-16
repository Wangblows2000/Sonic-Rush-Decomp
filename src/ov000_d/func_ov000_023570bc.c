#include <ov000_d/mdl.h>

extern void *data_0208fe80;

void func_02031768(UnkStruct_MdlRender *r);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkFree_023570bc(void **pp) {
    u32 size;
    void *p = *pp;
    if (p == NULL) {
        return;
    }
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
    *pp = NULL;
}

#pragma thumb on

void func_ov000_023570bc(UnkStruct_Mdl *w) {
    vu32 zero;

    func_02031768(&w->slot[0].r);
    func_02031768(&w->slot[1].r);
    UnkFree_023570bc(&w->unk2FC);
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Mdl));
}

#pragma thumb reset
