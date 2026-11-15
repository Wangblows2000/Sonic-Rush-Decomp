#include <nitro/types.h>

typedef struct UnkStruct_0231506c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231506c;

void func_ov000_02314e60(void);

void func_ov000_0231506c(UnkStruct_0231506c *s) {
    s->func = func_ov000_02314e60;
}
