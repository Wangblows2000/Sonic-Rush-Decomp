#include <nitro/types.h>

typedef struct UnkStruct_022f061c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3e8];
    u16 unk454;
} UnkStruct_022f061c;

void BossFPartD8_State_SkyChargeStart2(void);

void BossFPartD8_State_WaitTimer_ov000_022f061c(UnkStruct_022f061c *s) {
    if (s->unk454++ > 0x14)
        s->func = BossFPartD8_State_SkyChargeStart2;
}
