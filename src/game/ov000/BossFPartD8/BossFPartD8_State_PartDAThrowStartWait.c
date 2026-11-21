#include <nitro/types.h>

typedef struct UnkStruct_022f2bdc {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x57c - 0x6c];
    u8 unk57c;
} UnkStruct_022f2bdc;

void BossFPartD8_State_PartDAThrowSetDelay(void);

void BossFPartD8_State_PartDAThrowStartWait(UnkStruct_022f2bdc *s) {
    if (s->unk57c & 2)
        s->func = BossFPartD8_State_PartDAThrowSetDelay;
}
