#include <nitro/types.h>

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022d585c(void) {
    Sound_PlaySeqArc(0, 0, 2);
}
#pragma thumb reset
