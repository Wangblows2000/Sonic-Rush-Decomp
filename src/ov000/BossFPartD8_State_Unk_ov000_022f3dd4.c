#include <nitro/types.h>

typedef struct UnkStruct_022f3dd4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f3dd4;

void func_ov000_022f4ce4(UnkStruct_022f3dd4 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f3dac(void);

void BossFPartD8_State_Unk_ov000_022f3dd4(UnkStruct_022f3dd4 *s) {
    func_ov000_022f4ce4(s, 7, 0x1000, 0);
    s->func = BossFPartD8_State_Unk_ov000_022f3dac;
}
