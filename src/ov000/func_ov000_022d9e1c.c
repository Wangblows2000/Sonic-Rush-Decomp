#include <nitro/types.h>

typedef struct UnkStruct_022d9e1c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x224 - 0x6c];
    u16 unk224;
} UnkStruct_022d9e1c;

extern s32 data_ov000_02369ac0[];

void func_ov000_022d9dc0(void);

void func_ov000_022d9e1c(UnkStruct_022d9e1c *s) {
    data_ov000_02369ac0[1] = 0xd;
    s->unk224 = 0;
    s->func = func_ov000_022d9dc0;
}
