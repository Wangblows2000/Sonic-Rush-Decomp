#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x24];
    s32 unk24;
    s32 unk28;
} UnkStruct_Pos;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x26E - 0x20C];
    u16 unk26E;
    u8 pad270[0x29A - 0x270];
    u16 unk29A;
    u8 pad29C[0x2F2 - 0x29C];
    u16 unk2F2;
    u8 pad2F4[0x31E - 0x2F4];
    u16 unk31E;
    u8 pad320[0x3D0 - 0x320];
    UnkStruct_Pos unk3D0;
    u8 pad3FC[0x400 - 0x3D0 - sizeof(UnkStruct_Pos)];
    UnkStruct_Pos unk400;
    u8 pad42C[0x434 - 0x400 - sizeof(UnkStruct_Pos)];
    union {
        s32 w;
        u16 h[2];
    } unk434;
    u8 pad438[0x57C - 0x438];
    u8 unk57C;
} UnkStruct_022f3080;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u8 data_ov000_02363e9c[][8];

s32 func_ov000_022fad3c(s32 a, s32 b, s32 c, u16 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_02328210(s32 a, s32 b, s32 c, s32 d);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022f300c(UnkStruct_022f3080 *s);

void func_ov000_022f3080(UnkStruct_022f3080 *s) {
    UnkStruct_Pos *p;
    BOOL b;
    s32 i;

    p = s->unk434.w ? &s->unk3D0 : &s->unk400;
    if (p->unk28 < -0x200000) {
        s->unk26E |= 4;
        s->unk2F2 |= 4;
    } else {
        s->unk26E &= ~4;
        s->unk2F2 &= ~4;
    }
    if (!(s->unk57C & 2))
        return;
    s->unk434.h[1] = 0x80;
    b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = data_022c4560.unk14;
    if (data_ov000_02363e9c[i][data_ov000_02369ac0.unk8 - 1] != 0) {
        func_ov000_022fad3c(0xdb, (s->unk400.unk24 >> 4) + 0xa000, 0x30000, 2, 0, 0, 0, 0, 0);
        func_ov000_022fad3c(0xdb, (s->unk3D0.unk24 >> 4) - 0xa000, 0x30000, 3, 0, 0, 0, 0, 0);
    }
    func_ov000_02328210(1, (s->unk3D0.unk24 + s->unk400.unk24) >> 5, 0x30000, 0);
    s->unk26E &= ~4;
    s->unk2F2 &= ~4;
    s->unk29A |= 4;
    s->unk31E |= 4;
    func_02036754(0, 0, 0xbf);
    s->base.unk68 = func_ov000_022f300c;
}
