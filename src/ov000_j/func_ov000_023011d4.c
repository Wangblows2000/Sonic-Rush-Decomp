#include <nitro/types.h>

typedef struct {
    u8 pad0[0x238];
    u32 unk238;
} UnkStruct_023011d4_o;

typedef struct {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5e];
    UnkStruct_023011d4_o *unk88;
} UnkStruct_023011d4;

void StageObj_SetAnimation(UnkStruct_023011d4 *p, s32 a);
void func_ov000_0230119c(UnkStruct_023011d4 *p);
void func_ov000_023010a4(UnkStruct_023011d4 *p);

void func_ov000_023011d4(UnkStruct_023011d4 *p) {
    UnkStruct_023011d4_o *o = p->unk88;

    StageObj_SetAnimation(p, 1);
    p->unk5C |= 4;
    if (o->unk238 & 0x80) {
        o->unk238 &= ~0x80;
        func_ov000_0230119c(p);
    } else {
        func_ov000_023010a4(p);
    }
}
