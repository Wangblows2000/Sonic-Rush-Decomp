#include <nitro/types.h>

typedef struct UnkStruct_022f0280 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3e8];
    u16 unk454;
} UnkStruct_022f0280;

void BossFPartD8_State_Unk_ov000_022f01f8(void);

void BossFPartD8_State_WaitTimer_ov000_022f0280(UnkStruct_022f0280 *s) {
    if (s->unk454++ > 0x3c)
        s->func = BossFPartD8_State_Unk_ov000_022f01f8;
}
