#include <nitro/types.h>

typedef struct UnkStruct_022e9f1c {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022e9f1c;

void func_ov000_022e9ef0(void);

s32 func_ov000_022e9f1c(UnkStruct_022e9f1c *s) {
    s->unk454 = 0x78;
    s->func = func_ov000_022e9ef0;
    return 0;
}
