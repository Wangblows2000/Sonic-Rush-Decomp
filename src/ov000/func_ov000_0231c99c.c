#include <nitro/types.h>

typedef struct UnkStruct_0231c99c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231c99c;

void func_ov000_0231c834(void);

void func_ov000_0231c99c(UnkStruct_0231c99c *s) {
    s->func = func_ov000_0231c834;
}
