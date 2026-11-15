#include <nitro/types.h>

typedef struct UnkStruct_02331f4c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02331f4c;

void func_ov000_02331d44(void);

void func_ov000_02331f4c(UnkStruct_02331f4c *s) {
    s->func = func_ov000_02331d44;
}
