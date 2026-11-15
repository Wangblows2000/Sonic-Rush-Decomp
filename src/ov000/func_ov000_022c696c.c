#include <nitro/types.h>

typedef struct UnkStruct_022c696c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c696c;

void func_ov000_022c6968(void);

void func_ov000_022c696c(UnkStruct_022c696c *s) {
    s->func = func_ov000_022c6968;
}
