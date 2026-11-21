#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x68 - 0x1C];
    void *unk68;
} UnkStruct_0232f6f4_P;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x44 - 0x1C];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_0232f6f4_P *unk88;
    u8 pad8C[0x9C - 0x8C];
    u16 unk9C;
} UnkStruct_0232f6f4;

void StageObj_SetAnimation(void *w, s32 a);
s32 ObjShift_Set(s32 v, s32 a, s32 b, s32 c, s32 d);
void Player_State_Swing(void);

void FixInformation_State_Swing(UnkStruct_0232f6f4 *w) {
    UnkStruct_0232f6f4_P *p = w->unk88;

    if (p == NULL)
        return;
    w->unk10 = p->unk10;
    w->unk14 = p->unk14;
    w->unk18 = p->unk18;
    if (w->unk5C & 2)
        w->unk14 = p->unk14 + 0x2000;
    else
        w->unk14 = p->unk14 - 0x2000;
    w->unkC++;
    if (w->unkC == 0x20) {
        if (w->unk9C == 4)
            StageObj_SetAnimation(w, 5);
        else
            StageObj_SetAnimation(w, 4);
        w->unk5C |= 4;
        w->unk46 = w->unk44 = 0x200;
    }
    if (w->unkC >= 0x20) {
        w->unk44 = ObjShift_Set(w->unk44, 0x100, 1, 0x20, 0);
        w->unk46 = w->unk44;
    }
    if (w->unkC > 0x78)
        w->unk54 |= 4;
    if (w->unk88->unk68 != Player_State_Swing)
        w->unk54 |= 4;
}
