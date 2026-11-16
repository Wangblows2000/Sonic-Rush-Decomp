#include <nitro/types.h>

typedef struct UnkStruct_022f5860 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad2[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f5860;

void BossFPartDE_State_WaitTimer_ov000_022f5840(void);

void BossFPartDE_State_Unk_ov000_022f5860(UnkStruct_022f5860 *s) {
    s->unkC = 0;
    s->func = BossFPartDE_State_WaitTimer_ov000_022f5840;
}
