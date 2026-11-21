#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetMapRectHFlip(s32 a, s32 b, s32 c, s32 d, void *e, s32 f);
void TalkDemo_FlipImage10x10(void) { TalkDemo_SetMapRectHFlip(0, 0, 10, 10, (void *)0x6002000, 1); }

#pragma thumb reset
