#include <nitro/types.h>

typedef struct UnkStruct_022f5840 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x58];
    void (*func)(void);
} UnkStruct_022f5840;

void BossFPartDE_State_ChainActivate(void);

void BossFPartDE_State_WaitTimer_ov000_022f5840(UnkStruct_022f5840 *s) {
    if (s->unkC++ > 0)
        s->func = BossFPartDE_State_ChainActivate;
}
