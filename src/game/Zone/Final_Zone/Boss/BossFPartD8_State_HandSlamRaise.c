#include <nitro/types.h>

typedef struct UnkStruct_022f3dd4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f3dd4;

void BossFPartD8_SetAnimation(UnkStruct_022f3dd4 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_HandSlamRaiseWait(void);

void BossFPartD8_State_HandSlamRaise(UnkStruct_022f3dd4 *s) {
    BossFPartD8_SetAnimation(s, 7, 0x1000, 0);
    s->func = BossFPartD8_State_HandSlamRaiseWait;
}
