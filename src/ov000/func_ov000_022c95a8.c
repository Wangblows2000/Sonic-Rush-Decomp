#include <nitro/types.h>

typedef struct UnkStruct_022c95a8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c95a8;

void func_ov000_022c94a0(void);

void func_ov000_022c95a8(UnkStruct_022c95a8 *s) {
    s->func = func_ov000_022c94a0;
}
