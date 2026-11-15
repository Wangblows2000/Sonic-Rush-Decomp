#include <nitro/types.h>

typedef struct UnkStruct_022c9d8c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c9d8c;

void func_ov000_022c9ce8(void);

void func_ov000_022c9d8c(UnkStruct_022c9d8c *s) {
    s->func = func_ov000_022c9ce8;
}
