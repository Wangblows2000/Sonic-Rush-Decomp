#include <nitro/types.h>

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x384 - 6];
    u8 unk384[0x3F];
} UnkStruct_02355874;

void Math_RotateVec2S16(s16 *x, s16 *y, u8 d);
void Player_Action_GimmickLaunch(UnkStruct_02355874 *w, s16 x, s16 y);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_PedestalLaunch(UnkStruct_02355874 *w, s16 x, s16 y) {
    Math_RotateVec2S16(&x, &y, w->unk5);
    Player_Action_GimmickLaunch(w, x, y);
    Sound_PlaySeqArcEx(w->unk384, 0, 0x3F);
}
