#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x390 - 0x20C];
    s32 unk390;
} UnkStruct_022ed9a0;

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_Spark;

s32 func_02002094(s32 n, s32 d);
UnkStruct_StageObj *func_ov000_02328390(s32 a);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

UnkStruct_StageObj *func_ov000_022ed9a0(UnkStruct_022ed9a0 *w) {
    UnkStruct_StageObj *o = func_ov000_02328390(1);
    s32 s;

    o->unk10 += ((Rand() & 0xf) - 8) << 8;
    o->unk14 += ((Rand() & 0xf) - 8) << 8;
    o->unk18 += ((Rand() & 0xf) - 8) << 8;
    s = (func_02002094(w->unk390 << 12, 0x80) >> 1) + 0x800;
    *(s32 *)((u8 *)o + 0x14C) = FX_Mul(FX_Mul((Rand() & 0xf) << 8, -0x1000) + 0x34cc, s);
    *(s32 *)((u8 *)o + 0x150) = FX_Mul(FX_Mul((Rand() & 0xf) << 8, -0x1000) + 0x34cc, s);
    return o;
}
