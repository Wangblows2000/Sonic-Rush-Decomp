#include <nitro/types.h>

typedef struct UnkStruct_022dc158 {
    u8 pad[0x3a8];
    s32 unk3A8;
    s32 unk3AC;
} UnkStruct_022dc158;

void Boss4PartD1_StartTurnToPlayer(UnkStruct_022dc158 *s);

void Boss4PartD1_SubState_Brake(UnkStruct_022dc158 *s) {
    if (s->unk3AC * s->unk3A8 < 0)
        return;
    Boss4PartD1_StartTurnToPlayer(s);
}
