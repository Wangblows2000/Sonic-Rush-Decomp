#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e73e0 UnkStruct_022e73e0;
struct UnkStruct_022e73e0 {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D4 - 0x20C];
    s32 unk3D4;
    u8 pad3D8[0x440 - 0x3D8];
    void (*unk440)(UnkStruct_022e73e0 *s);
    u8 pad444[0x464 - 0x444];
    s32 unk464;
    s32 unk468;
    s32 unk46C;
    s32 unk470;
    s32 unk474;
    s32 unk478;
    s16 unk47C;
};

extern UnkStruct_StageObj *data_ov000_0236ac60;
extern s16 data_0206e470[];

void func_ov000_022e73c4(UnkStruct_022e73e0 *s);
void func_ov000_022ee198(UnkStruct_022e73e0 *s, s32 st);
void func_ov000_022e9b70(UnkStruct_022e73e0 *s, s32 a, s32 b);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022e73e0(UnkStruct_022e73e0 *s) {
    u16 ang;
    s32 st;
    s32 sn;

    s->unk47C += 0x7a;
    if (s->unk47C >= 0x1000) {
        s->unk47C = 0x1000;
        s->unk440 = func_ov000_022e73c4;
    }
    ang = FX_Mul(s->unk47C, 0x8000000) >> 12;
    if (s->unk47C < 0x1000) {
        st = s->unk3D4;
        if (st < 0x13 || st > 0x15)
            func_ov000_022ee198(s, 0x14);
        else if (st == 0x13 && ang < 0x4000)
            func_ov000_022ee198(s, 0x14);
        else if (st == 0x14 && (s->base.unk5C & 8))
            func_ov000_022ee198(s, 0x15);
    } else if (s->unk3D4 != 0x3c) {
        func_ov000_022ee198(s, 0x3c);
    }
    func_ov000_022e9b70(s, 0, 0);
    sn = data_0206e470[((ang & 0xffff) >> 4) * 2];
    data_ov000_0236ac60->unk14 = (s->unk468 - s->unk46C * sn) >> 4;
    s->base.unk14 = (s->unk474 - s->unk478 * sn) >> 4;
    data_ov000_0236ac60->unk10 = (s->unk464 + FX_Mul(0x40000 - s->unk464, s->unk47C)) >> 4;
    s->base.unk10 = (s->unk470 + FX_Mul(0x100000 - s->unk470, s->unk47C)) >> 4;
}
