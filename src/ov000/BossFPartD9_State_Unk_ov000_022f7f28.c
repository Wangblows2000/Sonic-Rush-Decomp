#include <nitro/types.h>

typedef struct UnkStruct_022f7f28 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1b0];
    u16 unk21C;
} UnkStruct_022f7f28;

void BossFPartD9_State_Unk_ov000_022f7d74(void);

void BossFPartD9_State_Unk_ov000_022f7f28(UnkStruct_022f7f28 *s) {
    s->unk21C = 0;
    s->func = BossFPartD9_State_Unk_ov000_022f7d74;
}
