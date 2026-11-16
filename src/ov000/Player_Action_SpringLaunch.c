#include <nitro/types.h>

void Player_Action_Launch(u8 *p);
void Sound_PlaySeqArcEx(void *p, s32 v, s32 n);

void Player_Action_SpringLaunch(u8 *p) {
    Player_Action_Launch(p);
    Sound_PlaySeqArcEx(p + 0x384, 0, 0x42);
}
