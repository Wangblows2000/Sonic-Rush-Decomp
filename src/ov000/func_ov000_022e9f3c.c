#include <nitro/types.h>

typedef struct UnkStruct_022e9f3c {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x3e0];
    void (*func)(void);
} UnkStruct_022e9f3c;

void func_ov000_022e9df0(void);
void func_ov000_022e9f1c(void);

s32 func_ov000_022e9f3c(UnkStruct_022e9f3c *s) {
    if (s->unk58 & 1)
        s->func = func_ov000_022e9df0;
    else
        s->func = func_ov000_022e9f1c;
    return 0;
}
