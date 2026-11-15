#include <nitro/types.h>

typedef struct UnkStruct_022ea120 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x3e0];
    void (*func)(void);
} UnkStruct_022ea120;

void func_ov000_022ea0bc(void);
void func_ov000_022e9f8c(void);

s32 func_ov000_022ea120(UnkStruct_022ea120 *s) {
    if (s->unk58 & 1)
        s->func = func_ov000_022ea0bc;
    else
        s->func = func_ov000_022e9f8c;
    return 0;
}
