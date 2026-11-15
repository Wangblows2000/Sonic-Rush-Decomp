#include <nitro/types.h>

typedef struct UnkStruct_02300688 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x68 - 0x5c];
    void (*func)(void);
} UnkStruct_02300688;

void func_02069818(UnkStruct_02300688 *s, s32 a);
void func_ov000_023005bc(void);

void func_ov000_02300688(UnkStruct_02300688 *s) {
    func_02069818(s, 1);
    s->unk58 &= ~0x10;
    s->func = func_ov000_023005bc;
}
