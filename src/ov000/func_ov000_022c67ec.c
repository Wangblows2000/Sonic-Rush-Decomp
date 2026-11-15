#include <nitro/types.h>

typedef struct UnkStruct_022c67ec {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x254];
    u16 unk2C0;
} UnkStruct_022c67ec;

void func_ov000_022c67a4(void);

void func_ov000_022c67ec(UnkStruct_022c67ec *s) {
    if (s->unk2C0++ > 0x1e)
        s->func = func_ov000_022c67a4;
}
