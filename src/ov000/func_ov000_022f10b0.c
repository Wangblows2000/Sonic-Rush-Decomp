#include <nitro/types.h>

typedef struct UnkStruct_022f10b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f10b0;

void BossFPartD8_SetAnimation(UnkStruct_022f10b0 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f1064(void);

void func_ov000_022f10b0(UnkStruct_022f10b0 *s) {
    BossFPartD8_SetAnimation(s, 0x2b, 0x1000, 0);
    s->func = BossFPartD8_State_Unk_ov000_022f1064;
}
