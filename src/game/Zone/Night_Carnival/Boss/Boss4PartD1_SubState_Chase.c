#include <nitro/types.h>

typedef struct UnkStruct_022dc1a8 {
    u8 pad[0x348];
    s32 unk348;
} UnkStruct_022dc1a8;

s32 Boss4PartD1_GetPlayerDirection(UnkStruct_022dc1a8 *s);
void Boss4PartD1_StartBrake(UnkStruct_022dc1a8 *s);

void Boss4PartD1_SubState_Chase(UnkStruct_022dc1a8 *s) {
    if (s->unk348 == Boss4PartD1_GetPlayerDirection(s))
        return;
    Boss4PartD1_StartBrake(s);
}
