#include <nitro/types.h>

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

#pragma thumb on
void TitleDemo_PlaySeq1(void) {
    Sound_PlaySeqArc(0, 0, 1);
}
#pragma thumb reset
