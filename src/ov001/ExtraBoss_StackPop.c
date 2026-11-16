#include <nitro/types.h>

typedef struct { u8 pad[0xb8]; s32 sp; s32 stack[0x64]; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *data_ov001_0231f548;

s32 ExtraBoss_StackPop(void) {
    UnkStruct_0231f548 *t = data_ov001_0231f548;
    s32 v;
    if (t->sp >= 0x64) {
        return 0;
    }
    v = t->stack[t->sp];
    t->sp++;
    return v;
}
