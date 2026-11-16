#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022f499c_plr;

extern UnkStruct_022f499c_plr *gPlayers;

void BossFPartD8_MoveTowardX(void *p, s32 x, s32 a, s32 b, s32 c, s32 d, s32 e);

void BossFPartD8_TrackPlayerX_Offset(void *p, s32 b) {
    s32 x;
    UnkStruct_022f499c_plr *plr = gPlayers;

    if (b)
        x = plr->unk10 - 0x6400;
    else
        x = plr->unk10 + 0x6400;
    BossFPartD8_MoveTowardX(p, x, 0x4000, 0x300, 0x80, 0x40, 0x600);
}
