#include <nitro/types.h>

typedef struct UnkStruct_022f0c70 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0c70;

void BossFPartD8_State_Unk_ov000_022f0bec(void);

void BossFPartD8_State_Unk_ov000_022f0c70(UnkStruct_022f0c70 *s) {
    s->unk454 = 0;
    s->func = BossFPartD8_State_Unk_ov000_022f0bec;
}
