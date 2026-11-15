#include <nitro/types.h>

typedef struct UnkStruct_022d9c70 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9c70;

extern s32 data_ov000_02369ac0[];

void func_ov000_022d9c44(void);

void func_ov000_022d9c70(UnkStruct_022d9c70 *s) {
    data_ov000_02369ac0[1] = 0x12;
    s->func = func_ov000_022d9c44;
}
