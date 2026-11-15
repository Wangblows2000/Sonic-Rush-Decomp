#include <nitro/types.h>

typedef struct UnkStruct_022caff0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022caff0;

extern s32 data_ov000_02369ac0[];

void func_ov000_022cafc4(void);

void func_ov000_022caff0(UnkStruct_022caff0 *s) {
    data_ov000_02369ac0[1] = 2;
    s->func = func_ov000_022cafc4;
}
