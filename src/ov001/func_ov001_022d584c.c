#include <nitro/types.h>

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022d584c(void) {
    Sound_PlaySeqArc(0, 0, 3);
}
#pragma thumb reset
