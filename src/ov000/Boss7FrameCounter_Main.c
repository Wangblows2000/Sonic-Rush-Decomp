#include <nitro/types.h>

typedef struct UnkStruct_022ee420 {
    u8 pad[0x220];
    s32 unk220;
} UnkStruct_022ee420;

extern UnkStruct_022ee420 *gBoss7;

void Boss7FrameCounter_Main(void) {
    gBoss7->unk220++;
}
