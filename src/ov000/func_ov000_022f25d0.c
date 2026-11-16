#include <nitro/types.h>

typedef struct UnkStruct_022f25d0 UnkStruct_022f25d0;
struct UnkStruct_022f25d0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f25d0 *s);
};

void func_ov000_022f4ce4(UnkStruct_022f25d0 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f2594(UnkStruct_022f25d0 *s);

void func_ov000_022f25d0(UnkStruct_022f25d0 *s) {
    func_ov000_022f4ce4(s, 0x18, 0x1000, 0);
    s->func = BossFPartD8_State_Unk_ov000_022f2594;
    s->func(s);
}
