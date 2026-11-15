#include <nitro/types.h>

typedef struct UnkStruct_022ea090 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022ea090;

void func_ov000_022e9f8c(void);

s32 func_ov000_022ea090(UnkStruct_022ea090 *s) {
    s->unk454--;
    if (s->unk454 == 0)
        s->func = func_ov000_022e9f8c;
    return 1;
}
