#include <ov000_d/mdl.h>

extern void *data_0208fe80;
extern u8 data_ov000_023690d4[];

void func_02031768(UnkStruct_MdlRender *r);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02021310(void *buf, void *a, void *b);
void func_02021264(void *buf, s32 a);
void func_0202566c(void);
void func_020212e0(void *buf);

static inline void UnkFree_02357308(void **pp) {
    u32 size;
    void *p = *pp;
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

void func_ov000_02357308(UnkStruct_Mdl *w) {
    vu32 zero;
    u8 buf[0x60];

    if (w->unk2F8 != NULL) {
        func_02031768(&w->slot[0].r);
        func_02031768(&w->slot[1].r);
        func_02021310(buf, data_ov000_023690d4, w->unk2F8);
        func_02021264(buf, 0);
        func_0202566c();
        func_02021264(buf, 3);
        func_0202566c();
        func_020212e0(buf);
        UnkFree_02357308(&w->unk2F8);
        UnkFree_02357308(&w->unk2FC);
        if (w->unk300 != NULL) {
            UnkFree_02357308(&w->unk300);
        }
    }
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Mdl));
}

#pragma thumb reset
