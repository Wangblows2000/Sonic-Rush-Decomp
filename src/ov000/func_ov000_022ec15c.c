#include <nitro/types.h>

typedef struct UnkStruct_022ec15c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022ec15c;

void func_ov000_022ebf44(void);

void func_ov000_022ec15c(UnkStruct_022ec15c *s) {
    s->func = func_ov000_022ebf44;
}
