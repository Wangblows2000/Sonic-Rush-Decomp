#include <nitro/types.h>

typedef struct UnkStruct_022e3cb4 {
    u8 pad[0xa6c];
    u8 unkA6C;
    u8 pad1[0x111c - 0xa6d];
    s32 unk111C;
    s32 unk1120;
} UnkStruct_022e3cb4;

void Boss6_SelectNextAttack(s32 a);

void Boss6_State_WaitSelectAttack3(UnkStruct_022e3cb4 *s) {
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(3);
    s->unk111C = 0;
    s->unk1120 = 0;
}
