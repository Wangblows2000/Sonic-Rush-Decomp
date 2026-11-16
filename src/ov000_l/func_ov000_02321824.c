#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02321824_A;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x184 - 0x134];
    UnkStruct_02321824_A *unk184;
} UnkWork_02321824;

void func_ov000_0232179c(void);

void func_ov000_02321824(UnkWork_02321824 *w) {
    w->base.unk68 = func_ov000_0232179c;
    w->base.unk5C |= 4;
    w->base.unk58 &= ~0x2000;
    w->base.unk58 |= 0x100;
    w->base.unk58 |= 0x80;
    w->base.unk40 = 3;
    w->base.unk42 = 0x60;
    if (w->unk184->unk2 >= 0xab) {
        w->base.unk40 = 2;
        w->base.unk42 >>= 1;
    }
    w->base.unkC = 0x4f - (Rand() & 0x3e);
}
