#include <nitro/types.h>

typedef struct UnkStruct_0232e90c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0232e90c;

void func_ov000_0232e854(void);

void func_ov000_0232e90c(UnkStruct_0232e90c *s) {
    s->func = func_ov000_0232e854;
}
