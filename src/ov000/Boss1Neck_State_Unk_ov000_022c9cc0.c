#include <nitro/types.h>

typedef struct UnkStruct_022c9cc0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c8];
    u8 unk334;
} UnkStruct_022c9cc0;

void func_ov000_022ca00c(UnkStruct_022c9cc0 *s);
void func_ov000_022c9c24(void);

void Boss1Neck_State_Unk_ov000_022c9cc0(UnkStruct_022c9cc0 *s) {
    func_ov000_022ca00c(s);
    if (s->unk334 & 2)
        s->func = func_ov000_022c9c24;
}
