#include <ov000_c/stageobj.h>

extern void *gPlayers[];
extern u8 data_ov000_02366b44[];

void Player_AddRings(void *p, s32 a);
void Player_AddLives(void *p, s32 a);
void Player_AddTension(void *p, s32 a);
void Player_GiveInvincibility(void *p);
void Player_GiveBarrier(void *p);
void Player_GiveMagnet(void *p);
void Player_ApplySlowdown(void *p);
void Player_ApplyConfusion(void *p);
void Player_DrainTension(void *p);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void MapObject_Unk_ov000_022fa9e8(void *a, void *p, s32 b);

void ItemBox_Unk_ov000_0230da90(void *a, void *p, s32 k) {
    switch (k) {
    case 0:
    default:
        Player_AddRings(p, data_ov000_02366b44[(s16)(Rand() & 7)]);
        break;
    case 1:
        Player_AddRings(p, 5);
        break;
    case 3:
        Player_AddTension(p, 0x12c0);
        break;
    case 4:
        Player_AddTension(p, 0x640);
        break;
    case 2:
        Player_AddLives(p, 1);
        break;
    case 5:
        Player_GiveInvincibility(p);
        break;
    case 6:
        Player_GiveBarrier(p);
        break;
    case 7:
        Player_GiveMagnet(p);
        break;
    case 8:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        MapObject_Unk_ov000_022fa9e8(a, p, 7);
        Player_ApplySlowdown(gPlayers[1]);
        break;
    case 9:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        MapObject_Unk_ov000_022fa9e8(a, p, 8);
        Player_ApplyConfusion(gPlayers[1]);
        break;
    case 10:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        Player_AddTension(p, 0x12c0);
        Player_DrainTension(gPlayers[1]);
        MapObject_Unk_ov000_022fa9e8(a, p, 9);
        break;
    case 11:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        MapObject_Unk_ov000_022fa9e8(a, p, 10);
        break;
    }
}
