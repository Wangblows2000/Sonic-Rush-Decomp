#include <nitro/types.h>

typedef struct UnkStruct_022f7f80 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x68 - 0x1c];
    void (*func)(void);
} UnkStruct_022f7f80;

void BossEffect_CreateProduce(s32 a, s32 b, s32 c, s32 d);
void BossFPartD9_State_Unk_ov000_022f7f68(void);

void BossFPartD9_State_Unk_ov000_022f7f80(UnkStruct_022f7f80 *s) {
    BossEffect_CreateProduce(1, s->unk10, s->unk14, s->unk18 + 0x800);
    s->func = BossFPartD9_State_Unk_ov000_022f7f68;
}
