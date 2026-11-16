#include <nitro/types.h>

typedef struct UnkStruct_022f7f68 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad2[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f7f68;

void BossFPartD9_State_Unk_ov000_022f7f44(void);

void BossFPartD9_State_Unk_ov000_022f7f68(UnkStruct_022f7f68 *s) {
    s->unkC = 0;
    s->func = BossFPartD9_State_Unk_ov000_022f7f44;
}
