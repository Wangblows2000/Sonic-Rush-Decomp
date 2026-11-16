#include <nitro/types.h>

typedef struct UnkStruct_022f25d0 UnkStruct_022f25d0;
struct UnkStruct_022f25d0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f25d0 *s);
};

void BossFPartD8_SetAnimation(UnkStruct_022f25d0 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_PartDAThrowEndWait(UnkStruct_022f25d0 *s);

void BossFPartD8_State_PartDAThrowEnd(UnkStruct_022f25d0 *s) {
    BossFPartD8_SetAnimation(s, 0x18, 0x1000, 0);
    s->func = BossFPartD8_State_PartDAThrowEndWait;
    s->func(s);
}
