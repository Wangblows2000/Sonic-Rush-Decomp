#include <nitro/types.h>

typedef struct UnkStruct_022ebea4 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
    u8 pad2[0x20c - 0x6c];
    u8 unk20C[0x354 - 0x20c];
    s32 unk354;
    u8 pad3[0x360 - 0x354 - 4];
    s32 unk360;
} UnkStruct_022ebea4;

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022ebea0(void);

void func_ov000_022ebea4(UnkStruct_022ebea4 *s) {
    void *p = s->unk20C;
    s->unk5C |= 4;
    Model_SetAnimation(p, 0, s->unk354, 0, 0);
    Model_SetAnimation(p, 1, s->unk360, 0, 0);
    s->func = func_ov000_022ebea0;
}
