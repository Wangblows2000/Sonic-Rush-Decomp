#include <nitro/types.h>

void Sound_PlaySeq(s32 a, s32 b);

#pragma thumb on
void TitleScreen_PlayMusic(void) {
    Sound_PlaySeq(0, 0);
}
#pragma thumb reset
