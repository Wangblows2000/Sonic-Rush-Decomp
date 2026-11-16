#include <nitro/types.h>

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

#pragma thumb on
void TitleScreen_PlayStartSound(void) {
    Sound_PlaySeqArc(0, 0, 0);
}
#pragma thumb reset
