#include <nitro/types.h>

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
} UnkStruct_02345e98;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_02345e98_4560;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x24 - 0x1C];
    u32 unk24;
    u8 pad28[0x40 - 0x28];
    u32 unk40;
} UnkStruct_02345e98_4590;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_02345e98_4680;

extern UnkStruct_02345e98_4560 gGameState;
extern UnkStruct_02345e98_4590 data_022c4590;
extern UnkStruct_02345e98_4680 gSaveData;

void BrightnessFade_Create(s32 a, s32 b);
void Player_Action_ReachGoal(UnkStruct_02345e98 *w, s32 a);
void Player_Action_Die(UnkStruct_02345e98 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_CheckTimeOver(UnkStruct_02345e98 *w) {
    if (w->unk325 != 0) {
        return;
    }
    if (gGameState.unk10 == 1) {
        if (gGameState.unk20 >= 0x8c9f && !(w->unk32C & 0x1000000)) {
            BrightnessFade_Create(4, 0x80);
            Player_Action_ReachGoal(w, 0);
            data_022c4590.unk40 = 3;
        }
    } else if (!(gSaveData.unk8 & 2) || gGameState.unk10 == 2) {
        if (gGameState.unk20 >= 0x8c9f) {
            data_022c4590.unk18 = 0;
            data_022c4590.unk24 |= 4;
            Player_Action_Die(w);
        }
    }
    if (!(gGameState.unk18 & 1)) {
        return;
    }
    if (gGameState.unk20 < 0x87ef) {
        return;
    }
    if (gGameState.unk20 == 0x87ef || gGameState.unk20 == 0x8867 || gGameState.unk20 == 0x88df ||
        gGameState.unk20 == 0x8957 || gGameState.unk20 == 0x89cf || gGameState.unk20 == 0x8a47 ||
        gGameState.unk20 == 0x8abf || gGameState.unk20 == 0x8b37 || gGameState.unk20 == 0x8baf ||
        gGameState.unk20 == 0x8c27) {
        Sound_PlaySeqArcEx(NULL, 0, 0x36);
    }
}
