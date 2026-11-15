#include <nitro/types.h>

typedef struct UnkStruct_022d9d08 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9d08;

extern s32 data_ov000_02369ac0[];

void func_ov000_022d9cdc(void);

void func_ov000_022d9d08(UnkStruct_022d9d08 *s) {
    data_ov000_02369ac0[1] = 0x11;
    s->func = func_ov000_022d9cdc;
}
