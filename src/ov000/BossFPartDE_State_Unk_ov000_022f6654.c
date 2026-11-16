#include <nitro/types.h>

typedef struct UnkStruct_022f6654 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f6654;

void BossFPartDE_Unk_ov000_022f6b8c(UnkStruct_022f6654 *s);
void BossFPartDE_State_Unk_ov000_022f65fc(void);

void BossFPartDE_State_Unk_ov000_022f6654(UnkStruct_022f6654 *s) {
    BossFPartDE_Unk_ov000_022f6b8c(s);
    if (s->unkC++ > 0x1e)
        s->func = BossFPartDE_State_Unk_ov000_022f65fc;
}
