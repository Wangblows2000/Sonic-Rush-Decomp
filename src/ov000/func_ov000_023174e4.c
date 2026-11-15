#include <nitro/types.h>

typedef struct UnkStruct_023174e4 {
    u8 pad[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x4c];
    u16 unk5C;
    u8 pad2[0xa];
    void (*func)(void);
} UnkStruct_023174e4;

void func_ov000_023171b4(void);

void func_ov000_023174e4(UnkStruct_023174e4 *s) {
    s->func = func_ov000_023171b4;
    s->unkC = 0;
    s->unk8 = 0;
    s->unk5C |= 4;
}
