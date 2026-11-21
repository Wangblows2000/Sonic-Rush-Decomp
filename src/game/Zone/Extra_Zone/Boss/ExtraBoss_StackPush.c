#include <nitro/types.h>

typedef struct { u8 pad[0xb8]; s32 sp; s32 stack[0x64]; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *gExtraBossRingWork;

void ExtraBoss_StackPush(s32 v) {
    gExtraBossRingWork->sp--;
    gExtraBossRingWork->stack[gExtraBossRingWork->sp] = v;
}
