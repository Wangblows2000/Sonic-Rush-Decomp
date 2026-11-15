#include <nitro/types.h>

typedef struct UnkStruct_022d9cbc {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9cbc;

extern s32 data_ov000_02369ac0[];

void func_ov000_022d9c90(void);

void func_ov000_022d9cbc(UnkStruct_022d9cbc *s) {
    data_ov000_02369ac0[1] = 0x10;
    s->func = func_ov000_022d9c90;
}
