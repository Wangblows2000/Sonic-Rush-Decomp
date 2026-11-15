#include <nitro/types.h>

typedef struct UnkStruct_022d9da0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9da0;

extern s32 data_ov000_02369ac0[];

void func_ov000_022d9d74(void);

void func_ov000_022d9da0(UnkStruct_022d9da0 *s) {
    data_ov000_02369ac0[1] = 0xf;
    s->func = func_ov000_022d9d74;
}
