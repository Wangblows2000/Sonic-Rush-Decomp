#include <nitro/types.h>

typedef struct UnkStruct_02310e7c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02310e7c;

void func_ov000_02310e78(void);

void func_ov000_02310e7c(UnkStruct_02310e7c *s) {
    s->func = func_ov000_02310e78;
}
