#include <nitro/types.h>

typedef struct UnkStruct_02328908 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
} UnkStruct_02328908;

void EffEnemyRoomFix_State_Main(UnkStruct_02328908 *s) {
    if (s->unkC-- == 0)
        s->unk54 |= 4;
}
