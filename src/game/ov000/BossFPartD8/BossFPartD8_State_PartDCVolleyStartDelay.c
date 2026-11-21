#include <nitro/types.h>

typedef struct UnkStruct_022f18b0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3d6];
    u16 unk442;
} UnkStruct_022f18b0;

void BossFPartD8_State_PartDCVolleyDelay(void);

void BossFPartD8_State_PartDCVolleyStartDelay(UnkStruct_022f18b0 *s) {
    s->unk442 = 0;
    s->func = BossFPartD8_State_PartDCVolleyDelay;
}
