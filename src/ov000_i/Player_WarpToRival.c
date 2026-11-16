#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x330 - 0x18];
    u32 unk330;
} UnkObj_02342e28;

extern UnkObj_02342e28 *volatile gPlayers[];
typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x32c - 0x58];
    u32 unk32C;
    u8 pad330[0x408 - 0x330];
    s32 unk408;
    s32 unk40C;
} UnkStruct_02342e28;

void Player_Action_Warp(UnkStruct_02342e28 *w);

void Player_WarpToRival(UnkStruct_02342e28 *w) {
    if (w->unk32C & 0x1000000)
        return;
    if (gPlayers[1]->unk330 & 0x800000)
        w->unk54 |= 1;
    else
        w->unk54 &= ~1;
    w->unk408 = gPlayers[1]->x;
    w->unk40C = gPlayers[1]->y;
    Player_Action_Warp(w);
}
