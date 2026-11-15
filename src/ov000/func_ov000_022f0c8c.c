#include <nitro/types.h>

typedef struct UnkStruct_022f0c8c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3e8];
    u16 unk454;
} UnkStruct_022f0c8c;

void func_ov000_022f0c70(void);

void func_ov000_022f0c8c(UnkStruct_022f0c8c *s) {
    if (s->unk454++ > 0x78)
        s->func = func_ov000_022f0c70;
}
