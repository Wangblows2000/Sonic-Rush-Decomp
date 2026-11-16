#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x58 - 0x18];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
} UnkStruct_02311cc4_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[6];
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x88 - 0x64];
    UnkStruct_02311cc4_P *unk88;
} UnkStruct_02311cc4;

void Math_RotateVec2S16(s16 *x, s16 *y, u8 ang);
void StageObj_SetAnimation(void *w, s32 a);
u8 ObjSpd_DownSet(u8 a, s32 b);
s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);

void ParachuteReleased_State_Unk_ov000_02311cc4(UnkStruct_02311cc4 *w) {
    s16 x;
    s16 y;
    UnkStruct_02311cc4_P *p = w->unk88;

    if (p != NULL) {
        x = 0;
        y = -0x3000;
        if (w->unk88->unk5C & 1)
            w->unk5C |= 1;
        else
            w->unk5C &= ~1;
        w->unk1 = w->unk88->unk1;
        Math_RotateVec2S16(&x, &y, w->unk1);
        w->unk10 = w->unk88->unk10 + x;
        w->unk14 = w->unk88->unk14 + y;
        if ((w->unk88->unk58 & 1) || (w->unk88->unk58 & 0x2000) || (w->unk88->unk58 & 0x100)) {
            w->unk88 = NULL;
            w->unk58 &= ~0x2000;
            return;
        }
        if (w->unkC != 0)
            return;
        if (w->unk88->unk0 != 1)
            return;
        if (p->unk328 != 0x15)
            return;
        StageObj_SetAnimation(w, 1);
        w->unkC = 1;
        w->unk62 = 0x60;
        return;
    }
    if (w->unk54 & 0x10)
        StageObj_SetAnimation(w, 2);
    w->unk54 &= ~0x10;
    w->unk8++;
    if (w->unk8 > 0x20) {
        if (w->unk8 & 2)
            w->unk5C |= 0x20;
        else
            w->unk5C &= ~0x20;
    }
    w->unk1 = ObjSpd_DownSet(w->unk1, 1);
    w->unk2E = ObjSpd_UpSet(w->unk2E, 8, 0x180);
    w->unk30 = ObjSpd_UpSet(w->unk30, -4, 0x200);
    if (w->unk8 > 0x40)
        w->unk54 |= 4;
}
