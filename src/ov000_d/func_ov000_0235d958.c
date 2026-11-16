#include <ov000_d/mdl2.h>

extern void *data_0208fe80;
extern u8 data_ov000_023692f4[];

void func_02031768(UnkStruct_MdlRender *r);
BOOL func_020513a8(void);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02021310(void *buf, void *a, void *b);
void func_02021264(void *buf, s32 a);
void func_0202566c(void);
void func_020212e0(void *buf);

static inline void UnkFree_0235d958(void **pp) {
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

void func_ov000_0235d958(UnkStruct_Mdl2 *w) {
    u8 buf[0x5c];

    if (func_020513a8()) {
        func_02031768(&w->rA);
        func_02031768(&w->rB);
        UnkFree_0235d958(&w->unk90C);
        return;
    }
    if (w->unk908 != NULL) {
        func_02031768(&w->rA);
        func_02031768(&w->rB);
        func_02021310(buf, data_ov000_023692f4, w->unk908);
        func_02021264(buf, 0);
        func_0202566c();
        func_02021264(buf, 3);
        func_0202566c();
        func_020212e0(buf);
        UnkFree_0235d958(&w->unk908);
        UnkFree_0235d958(&w->unk90C);
    }
}

#pragma thumb reset
