#include <nitro/types.h>

typedef struct UnkStruct_022f1b4c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f1b4c;

void BossFPartD8_Unk_ov000_022f89a8(UnkStruct_022f1b4c *s);
void BossFPartD8_State_HandRideRetract(void);

void BossFPartD8_State_HandRideEndWait(UnkStruct_022f1b4c *s) {
    BossFPartD8_Unk_ov000_022f89a8(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_HandRideRetract;
}
