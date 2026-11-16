#include <nitro/types.h>

typedef struct UnkStruct_022f2e20 UnkStruct_022f2e20;
struct UnkStruct_022f2e20 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f2e20 *s);
};

void func_ov000_022f4ce4(UnkStruct_022f2e20 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f2de0(UnkStruct_022f2e20 *s);

void BossFPartD8_State_Unk_ov000_022f2e20(UnkStruct_022f2e20 *s) {
    func_ov000_022f4ce4(s, 0x14, 0x1000, 0);
    s->func = BossFPartD8_State_Unk_ov000_022f2de0;
    s->func(s);
}
