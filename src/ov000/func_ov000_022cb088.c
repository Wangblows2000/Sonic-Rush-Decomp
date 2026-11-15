#include <nitro/types.h>

typedef struct UnkStruct_022cb088 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb088;

extern s32 data_ov000_02369ac0[];

void func_ov000_022cb05c(void);

void func_ov000_022cb088(UnkStruct_022cb088 *s) {
    data_ov000_02369ac0[1] = 3;
    s->func = func_ov000_022cb05c;
}
