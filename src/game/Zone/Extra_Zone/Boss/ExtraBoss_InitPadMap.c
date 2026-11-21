#include <nitro/types.h>

typedef struct UnkStruct_022ffcb8 {
    u8 pad0[0x22];
    u16 masks[12];
} UnkStruct_022ffcb8;

extern UnkStruct_022ffcb8 *gExtraBossWork;

void ExtraBoss_InitPadMap(void) {
    s32 i;

    for (i = 0; i < 12; i++)
        gExtraBossWork->masks[i] = 1 << i;
}
