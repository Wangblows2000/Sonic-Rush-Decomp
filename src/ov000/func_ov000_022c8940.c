#include <nitro/types.h>

typedef struct UnkStruct_022c8940 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x210 - 0x6c];
    u16 unk210;
} UnkStruct_022c8940;

void func_ov000_022c889c(void);

void func_ov000_022c8940(UnkStruct_022c8940 *s) {
    s->unk210 = 0;
    s->func = func_ov000_022c889c;
}
