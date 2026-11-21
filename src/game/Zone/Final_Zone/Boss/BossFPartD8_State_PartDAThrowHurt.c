#include <nitro/types.h>

typedef struct UnkStruct_022f2524 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f2524;

void BossFPartD8_SetAnimation(UnkStruct_022f2524 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_PartDAThrowHurtWait(void);

void BossFPartD8_State_PartDAThrowHurt(UnkStruct_022f2524 *s) {
    BossFPartD8_SetAnimation(s, 0x19, 0x1000, 0);
    s->func = BossFPartD8_State_PartDAThrowHurtWait;
}
