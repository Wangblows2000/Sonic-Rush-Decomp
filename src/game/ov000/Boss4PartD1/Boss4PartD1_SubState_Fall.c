#include <nitro/types.h>

typedef struct UnkStruct_022dc2b8 {
    u8 pad[0x14];
    s32 unk14;
} UnkStruct_022dc2b8;

void Boss4PartD1_StartTurnToPlayer(UnkStruct_022dc2b8 *s);

void Boss4PartD1_SubState_Fall(UnkStruct_022dc2b8 *s) {
    if (s->unk14 < 0x20000)
        return;
    s->unk14 = 0x20000;
    Boss4PartD1_StartTurnToPlayer(s);
}
