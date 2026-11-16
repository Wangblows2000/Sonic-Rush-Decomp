#include <nitro/types.h>

typedef struct UnkStruct_022e73c4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022e73c4;

extern UnkStruct_022e73c4 *gBoss7;

void func_ov000_022e73c4(void) {
    gBoss7->unk20C |= 2;
}
