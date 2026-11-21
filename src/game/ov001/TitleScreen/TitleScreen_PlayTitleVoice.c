#include <nitro/types.h>

void Sound_PlayVoice(s32 a, s32 b, s32 c);

#pragma thumb on
void TitleScreen_PlayTitleVoice(void) {
    Sound_PlayVoice(0, 0, 5);
}
#pragma thumb reset
