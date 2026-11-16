#include <nitro/types.h>

typedef struct UnkStruct_022c9478 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c8];
    u8 unk334;
} UnkStruct_022c9478;

void func_ov000_022c9fbc(UnkStruct_022c9478 *s);
void func_ov000_022c93bc(void);

void Boss1Neck_State_Unk_ov000_022c9478(UnkStruct_022c9478 *s) {
    func_ov000_022c9fbc(s);
    if (s->unk334 & 2)
        s->func = func_ov000_022c93bc;
}
