#include <nitro/types.h>

typedef struct UnkStruct_022f7f44 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x58];
    void (*func)(void);
} UnkStruct_022f7f44;

void BossFPartD9_State_Unk_ov000_022f7f28(void);

void BossFPartD9_State_Unk_ov000_022f7f44(UnkStruct_022f7f44 *s) {
    s->unkC++;
    if (s->unkC > 10)
        s->func = BossFPartD9_State_Unk_ov000_022f7f28;
}
