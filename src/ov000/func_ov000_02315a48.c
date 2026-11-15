#include <nitro/types.h>

typedef struct UnkStruct_02315a48 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0xc];
    void (*func)(void);
} UnkStruct_02315a48;

void func_ov000_02315928(void);

void func_ov000_02315a48(UnkStruct_02315a48 *s) {
    s->func = func_ov000_02315928;
    s->unk58 |= 0x10;
}
