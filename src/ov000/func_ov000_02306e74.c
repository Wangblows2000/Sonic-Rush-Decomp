#include <nitro/types.h>

typedef struct UnkStruct_02306e74 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_02306e74;

void func_ov000_02306e48(void);

void func_ov000_02306e74(UnkStruct_02306e74 *s) {
    s->unk5C |= 4;
    s->func = func_ov000_02306e48;
}
