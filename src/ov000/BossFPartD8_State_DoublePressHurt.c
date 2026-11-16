#include <nitro/types.h>

typedef struct UnkStruct_022f2e20 UnkStruct_022f2e20;
struct UnkStruct_022f2e20 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f2e20 *s);
};

void BossFPartD8_SetAnimation(UnkStruct_022f2e20 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_DoublePressHurtWait(UnkStruct_022f2e20 *s);

void BossFPartD8_State_DoublePressHurt(UnkStruct_022f2e20 *s) {
    BossFPartD8_SetAnimation(s, 0x14, 0x1000, 0);
    s->func = BossFPartD8_State_DoublePressHurtWait;
    s->func(s);
}
