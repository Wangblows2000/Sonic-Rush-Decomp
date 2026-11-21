#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[5];
    s8 unk7;
    u8 pad8[0x2E - 8];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void *unk68;
    u8 pad6C[0x184 - 0x6C];
    UnkStruct_ObjParam *unk184;
} UnkStruct_0231dbc4;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_0231dbc4_Sys;

extern UnkStruct_0231dbc4_Sys gGameState;

u8 ObjAngle_MoveToward8(u8 a, s32 b, s32 c);
s16 ObjSpd_UpSet(s16 a, s16 b, s32 c);
void DecAnimal_State_Leave(void);

void DecAnimal_UpdateMove(UnkStruct_0231dbc4 *w) {
    s16 acc = 0x24;
    s16 max = 0x200;
    s32 v;

    switch (w->unk184->unk2) {
    case 10:
        max = 0x280;
        acc = 0x30;
        break;
    case 4:
        max = 0x2a0;
        acc = 0x2a;
        break;
    case 14:
        max = 0x180;
        break;
    case 3:
    case 8:
    case 13:
        acc = 0x80;
        if (w->unk58 & 1) {
            if (w->unk184->unk2 == 8) {
                w->unk30 = -0x300;
                max = 0x240;
            } else if (w->unk184->unk2 == 13) {
                w->unk30 = -0x180;
                max = 0x280;
            } else {
                w->unk30 = -0x240;
            }
            w->unk58 |= 0x10;
        }
        break;
    case 12:
        max = 0x400;
        break;
    case 7:
        if (w->unk5C & 0x10) {
            w->unk2E = 0;
            if (w->unk62 == 0 && !(gGameState.unk20 & 0x1f))
                w->unk5C &= ~0x10;
        } else if (w->unk1 != 0) {
            w->unk1 = ObjAngle_MoveToward8(w->unk1, 0, 4);
            w->unk2E = 0;
        } else {
            w->unk58 &= ~0x2100;
            if (!(gGameState.unk20 & 0xf) && (w->unk58 & 1) && !(Rand() & 0xf)) {
                w->unk1 = 0x40;
                if (w->unk5C & 1)
                    w->unk1 += 0x80;
                w->unk2E = 0;
                w->unk5C |= 0x10;
                w->unk58 |= 0x2100;
                w->unk62 = 0x40;
            }
        }
        v = (s8)w->unk1;
        if (v < 0)
            v = -v;
        w->unk7 = -(v >> 4);
        break;
    }
    if (w->unk58 & 4)
        w->unk5C ^= 1;
    if (w->unk68 != DecAnimal_State_Leave)
        max = max - (max >> 2);
    if (w->unk5C & 1)
        w->unk2E = ObjSpd_UpSet(w->unk2E, -acc, max);
    else
        w->unk2E = ObjSpd_UpSet(w->unk2E, acc, max);
}
