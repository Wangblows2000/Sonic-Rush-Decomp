#include <nitro/types.h>

typedef struct UnkStruct_022cb03c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb03c;

extern s32 data_ov000_02369ac0[];

void func_ov000_022cb010(void);

void func_ov000_022cb03c(UnkStruct_022cb03c *s) {
    data_ov000_02369ac0[1] = 4;
    s->func = func_ov000_022cb010;
}
