#include <nitro/types.h>

typedef struct UnkStruct_022f133c UnkStruct_022f133c;
struct UnkStruct_022f133c {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f133c *s);
};

void BossFPartD8_SetAnimation(UnkStruct_022f133c *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_PartDCVolleyEndWait(UnkStruct_022f133c *s);

void BossFPartD8_State_PartDCVolleyEnd(UnkStruct_022f133c *s) {
    BossFPartD8_SetAnimation(s, 0x28, 0x1000, 0);
    s->func = BossFPartD8_State_PartDCVolleyEndWait;
    s->func(s);
}
