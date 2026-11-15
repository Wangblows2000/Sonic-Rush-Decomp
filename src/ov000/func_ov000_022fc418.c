#include <nitro/types.h>

typedef struct UnkStruct_022fc418 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x5c - 0x10];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_022fc418;

void func_ov000_022faf48(UnkStruct_022fc418 *s, s32 a);
void func_ov000_022fc398(void);

void func_ov000_022fc418(UnkStruct_022fc418 *s) {
    func_ov000_022faf48(s, 3);
    s->unk5C |= 4;
    s->func = func_ov000_022fc398;
    s->unkC = 0x10;
}
