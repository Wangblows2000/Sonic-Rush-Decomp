#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} UnkStruct_02344940_4534;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02344940_4560;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_02344940_3E8;

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
    u8 pad326[0x330 - 0x326];
    u32 unk330;
    u8 pad334[0x3E8 - 0x334];
    UnkStruct_02344940_3E8 *unk3E8;
} UnkStruct_02344940;

extern UnkStruct_02344940_4534 gStageMapCollision;
extern UnkStruct_02344940_4560 gGameState;

void Player_UpdateGimmickStageBounds(UnkStruct_02344940 *w) {
    if (w->unk325 != 0) {
        return;
    }
    if (gGameState.unk1C >= 9) {
        return;
    }
    if (gGameState.unk1D >= 2) {
        return;
    }
    if (w->unk330 & 0x200) {
        if (w->unk3E8 != NULL) {
            gStageMapCollision.unk1C = (w->unk3E8->unk10 >> 8) - 0x6c;
            gStageMapCollision.unk24 = (w->unk3E8->unk10 >> 8) + 0x6c;
        } else {
            w->unk330 &= ~0x200;
            gStageMapCollision.unk1C = 0;
            gStageMapCollision.unk24 = gStageMapCollision.unk18 << 6;
        }
    } else {
        gStageMapCollision.unk1C = 0;
        gStageMapCollision.unk24 = gStageMapCollision.unk18 << 6;
    }
    if (w->unk330 & 0x400) {
        if (w->unk3E8 != NULL) {
            gStageMapCollision.unk20 = (w->unk3E8->unk14 >> 8) - 0x58;
            gStageMapCollision.unk28 = (w->unk3E8->unk14 >> 8) + 0x58;
        } else {
            w->unk330 &= ~0x400;
            gStageMapCollision.unk20 = 0;
            gStageMapCollision.unk28 = gStageMapCollision.unk1A << 6;
        }
    } else {
        gStageMapCollision.unk20 = 0;
        gStageMapCollision.unk28 = gStageMapCollision.unk1A << 6;
    }
}
