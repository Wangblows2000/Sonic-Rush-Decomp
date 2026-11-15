#include <nitro/types.h>

typedef struct UnkStruct_0232bc28 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x48];
    u32 unk58;
    u8 pad2[0xc];
    void (*func)(void);
} UnkStruct_0232bc28;

void func_ov000_0232bb3c(void);

void func_ov000_0232bc28(UnkStruct_0232bc28 *s) {
    s->func = func_ov000_0232bb3c;
    if (s->unkC != 0)
        s->unk58 |= 0x2000;
}
