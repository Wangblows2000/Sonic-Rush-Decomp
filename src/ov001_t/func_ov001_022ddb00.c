#include <nitro/types.h>

typedef struct {
    u8 pad0[0x44];
    u16 unk44;
    u16 unk46;
} UnkSpr_022ddb00;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022ddb00 spr[29];
} UnkStruct_022ddb00;

void func_0203344c(UnkSpr_022ddb00 *s, u32 a, u32 b);

#pragma thumb on
void func_ov001_022ddb00(UnkStruct_022ddb00 *p) {

    p->spr[25].unk44 = 13;
    p->spr[26].unk44 = 13;
    p->spr[27].unk44 = 13;
    p->spr[28].unk44 = 13;
    func_0203344c(&p->spr[10], 0, 0);
    func_0203344c(&p->spr[13], 0, 0);
    func_0203344c(&p->spr[14], 0, 0);
    func_0203344c(&p->spr[15], 0, 0);
    func_0203344c(&p->spr[16], 0, 0);
    func_0203344c(&p->spr[17], 0, 0);
    func_0203344c(&p->spr[18], 0, 0);
    func_0203344c(&p->spr[19], 0, 0);
    func_0203344c(&p->spr[20], 0, 0);
    func_0203344c(&p->spr[21], 0, 0);
    func_0203344c(&p->spr[22], 0, 0);
    func_0203344c(&p->spr[23], 0, 0);
    func_0203344c(&p->spr[24], 0, 0);
    func_0203344c(&p->spr[25], 0, 0);
    func_0203344c(&p->spr[26], 0, 0);
    func_0203344c(&p->spr[27], 0, 0);
    func_0203344c(&p->spr[28], 0, 0);
}
#pragma thumb reset
