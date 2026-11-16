#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetMapRectHFlip(s32 a, s32 b, s32 c, s32 d, void *e, s32 f);
void TalkDemo_FlipImage22x10(void) { TalkDemo_SetMapRectHFlip(0, 0, 0x16, 10, (void *)0x6003000, 1); TalkDemo_SetMapRectHFlip(3, 1, 0x10, 7, (void *)0x6003000, 0); }

#pragma thumb reset
