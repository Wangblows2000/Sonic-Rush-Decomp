#include <nitro/types.h>

typedef struct UnkStruct_022f0280 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3e8];
    u16 unk454;
} UnkStruct_022f0280;

void func_ov000_022f01f8(void);

void func_ov000_022f0280(UnkStruct_022f0280 *s) {
    if (s->unk454++ > 0x3c)
        s->func = func_ov000_022f01f8;
}
