#include <nitro/types.h>

typedef struct UnkStruct_022f0bd0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0bd0;

void func_ov000_022f0b44(void);

void func_ov000_022f0bd0(UnkStruct_022f0bd0 *s) {
    s->unk454 = 0;
    s->func = func_ov000_022f0b44;
}
