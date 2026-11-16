#include <nitro/types.h>

typedef struct UnkStruct_022f09b4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3e8];
    u16 unk454;
} UnkStruct_022f09b4;

void func_ov000_022f0968(void);

void BossFPartD8_State_Unk_ov000_022f09b4(UnkStruct_022f09b4 *s) {
    if (s->unk454++ > 0x3c) {
        s->unk454 = 0;
        s->func = func_ov000_022f0968;
    }
}
