#include <nitro/types.h>

typedef struct UnkStruct_022f2ee0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f2ee0;

void BossFPartD8_SetAnimation(UnkStruct_022f2ee0 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_DoublePressEndWait(void);

void BossFPartD8_State_DoublePressEnd(UnkStruct_022f2ee0 *s) {
    BossFPartD8_SetAnimation(s, 0x13, 0x1000, 0);
    s->func = BossFPartD8_State_DoublePressEndWait;
}
