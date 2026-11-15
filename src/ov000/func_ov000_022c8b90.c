#include <nitro/types.h>

typedef struct UnkStruct_022c8b90 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1a0];
    u16 unk20C;
} UnkStruct_022c8b90;

void func_ov000_022c8b50(void);

void func_ov000_022c8b90(UnkStruct_022c8b90 *s) {
    if (s->unk20C++ > 5)
        s->func = func_ov000_022c8b50;
}
