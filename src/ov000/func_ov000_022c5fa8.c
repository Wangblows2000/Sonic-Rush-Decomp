#include <nitro/types.h>

typedef struct UnkStruct_022c5fa8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c5fa8;

void func_ov000_022c5fa4(void);

void func_ov000_022c5fa8(UnkStruct_022c5fa8 *s) {
    s->func = func_ov000_022c5fa4;
}
