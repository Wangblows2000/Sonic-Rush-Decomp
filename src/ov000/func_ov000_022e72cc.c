#include <nitro/types.h>

typedef struct UnkStruct_022e72cc {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x3e4];
    void (*func)(void);
} UnkStruct_022e72cc;

void func_ov000_022e98d8(void);

void func_ov000_022e72cc(UnkStruct_022e72cc *s) {
    if (s->unk58 & 1) {
        s->unk58 &= ~0x10;
        s->func = func_ov000_022e98d8;
    }
}
