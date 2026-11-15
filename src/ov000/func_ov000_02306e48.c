#include <nitro/types.h>

typedef struct UnkStruct_02306e48 {
    u8 pad[0x13e];
    u16 unk13E;
} UnkStruct_02306e48;

void func_0206bd24(UnkStruct_02306e48 *s, void *a, void *b);

void func_ov000_02306e48(UnkStruct_02306e48 *s) {
    func_0206bd24(s, (u8 *)s + 0x20c, (u8 *)s + 0x254);
    s->unk13E &= ~0x100;
}
