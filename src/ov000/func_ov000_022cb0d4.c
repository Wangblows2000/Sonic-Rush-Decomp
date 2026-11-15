#include <nitro/types.h>

typedef struct UnkStruct_022cb0d4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb0d4;

extern s32 data_ov000_02369ac0[];

void func_ov000_022cb0a8(void);

void func_ov000_022cb0d4(UnkStruct_022cb0d4 *s) {
    data_ov000_02369ac0[1] = 2;
    s->func = func_ov000_022cb0a8;
}
