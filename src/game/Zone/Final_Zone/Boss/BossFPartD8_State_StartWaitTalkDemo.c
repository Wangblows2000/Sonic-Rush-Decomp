#include <nitro/types.h>

typedef struct UnkStruct_022f0bd0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0bd0;

void BossFPartD8_State_WaitTalkDemo(void);

void BossFPartD8_State_StartWaitTalkDemo(UnkStruct_022f0bd0 *s) {
    s->unk454 = 0;
    s->func = BossFPartD8_State_WaitTalkDemo;
}
