#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_0235a0a4_Res;

typedef struct {
    u8 pad0[0x40];
} UnkStruct_0235a0a4_Obj;

typedef struct {
    u8 pad0[0x344];
    UnkStruct_0235a0a4_Obj obj[10];
    u8 pad5C4[0xCF4 - 0x5C4];
    UnkStruct_0235a0a4_Res *res[10];
} UnkStruct_0235a0a4;

void func_0203473c(UnkStruct_0235a0a4_Obj *o, UnkStruct_0235a0a4_Res *r, s32 a, s32 b, s32 c, u32 w, u32 h);
void func_0203438c(UnkStruct_0235a0a4_Obj *o);
void func_02002ab0(s32 a, s32 b, s32 c);

static inline void InitObj_0235a0a4(UnkStruct_0235a0a4_Obj *o, UnkStruct_0235a0a4_Res *r, s32 a, s32 b, s32 c) {
    func_0203473c(o, r, a, b, c, r->unk18, r->unk1A);
}

#pragma thumb on

void func_ov000_0235a0a4(UnkStruct_0235a0a4 *w) {
    InitObj_0235a0a4(&w->obj[6], w->res[6], 0x38, 0, 2);
    func_0203438c(&w->obj[6]);
    InitObj_0235a0a4(&w->obj[8], w->res[8], 0x38, 0, 3);
    func_0203438c(&w->obj[8]);
    func_02002ab0(1, 0, 1);
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x69c;
    InitObj_0235a0a4(&w->obj[9], w->res[9], 0x38, 0, 3);
    func_0203438c(&w->obj[9]);
    func_02002ab0(1, 3, 1);
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x4418;
    InitObj_0235a0a4(&w->obj[0], w->res[0], 0, 1, 0);
    InitObj_0235a0a4(&w->obj[1], w->res[1], 0, 1, 0);
    InitObj_0235a0a4(&w->obj[2], w->res[2], 0, 1, 0);
    InitObj_0235a0a4(&w->obj[3], w->res[3], 0, 1, 2);
    InitObj_0235a0a4(&w->obj[4], w->res[4], 0, 1, 2);
    InitObj_0235a0a4(&w->obj[5], w->res[5], 0, 1, 2);
    InitObj_0235a0a4(&w->obj[7], w->res[7], 0x38, 1, 3);
    func_0203438c(&w->obj[7]);
}

#pragma thumb reset
