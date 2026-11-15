#include <nitro/types.h>

typedef struct UnkStruct_0230613c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230613c;

void func_ov000_02305ea0(void);

void func_ov000_0230613c(UnkStruct_0230613c *s) {
    s->func = func_ov000_02305ea0;
}
