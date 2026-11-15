#include <nitro/types.h>

typedef struct UnkStruct_022cafa4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cafa4;

extern s32 data_ov000_02369ac0[];

void func_ov000_022caf78(void);

void func_ov000_022cafa4(UnkStruct_022cafa4 *s) {
    data_ov000_02369ac0[1] = 0x1b;
    s->func = func_ov000_022caf78;
}
