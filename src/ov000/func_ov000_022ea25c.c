#include <nitro/types.h>

typedef struct UnkStruct_022ea25c {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022ea25c;

void func_ov000_022ea20c(void);

s32 func_ov000_022ea25c(UnkStruct_022ea25c *s) {
    s->unk454 = 0;
    s->func = func_ov000_022ea20c;
    return 0;
}
