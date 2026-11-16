#include <nitro/types.h>

typedef struct UnkStruct_022cb0f4 {
    u8 pad[0x220];
    u16 unk220;
} UnkStruct_022cb0f4;

void Boss1_SelectNextAttack(void);

void Boss1_State_Idle(UnkStruct_022cb0f4 *s) {
    if (s->unk220 != 0) {
        s->unk220--;
        return;
    }
    Boss1_SelectNextAttack();
}
