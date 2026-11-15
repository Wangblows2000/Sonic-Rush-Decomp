#include <nitro/types.h>

typedef struct UnkStruct_0231ac10 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(void);
} UnkStruct_0231ac10;

void func_ov000_0231a8e8(UnkStruct_0231ac10 *s);
void func_ov000_0231abe8(void);

void func_ov000_0231ac10(UnkStruct_0231ac10 *s) {
    s->unk54 |= 2;
    func_ov000_0231a8e8(s);
    s->func = func_ov000_0231abe8;
}
