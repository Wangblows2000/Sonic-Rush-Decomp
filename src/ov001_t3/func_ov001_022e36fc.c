#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022e36fc;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    UnkSpr_022e36fc spr[6];
} UnkStruct_022e36fc;

void func_ov001_022e855c(UnkSpr_022e36fc *obj, s32 a, u16 id, s32 b, s32 c);
void func_0203344c(UnkSpr_022e36fc *s, s32 a, s32 b);

#pragma thumb on
void func_ov001_022e36fc(UnkStruct_022e36fc *p, s32 a, s32 b, s32 c) {
    s32 i;

    p->unk0 = 0;
    p->unk4 = 0;
    p->unk8 = 0;
    for (i = 0; i < 4; i++) {
        func_ov001_022e855c(&p->spr[i], a, i, 0xd, 0);
        p->spr[i].flags &= ~0x60000;
        p->spr[i].flags |= 4;
        func_0203344c(&p->spr[i], 0, 0);
    }
    func_ov001_022e855c(&p->spr[4], b, 0, 0xd, 0);
    p->spr[4].flags &= ~0x60000;
    p->spr[4].flags |= 4;
    func_ov001_022e855c(&p->spr[5], c, 4, 0xd, 0);
    p->spr[5].flags &= ~0x60000;
    p->spr[5].flags |= 4;
}
#pragma thumb reset
