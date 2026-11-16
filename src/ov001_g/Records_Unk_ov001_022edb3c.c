#include <nitro/types.h>

typedef struct UnkSpr_022edb3c {
    u8 pad0[0x48];
} UnkSpr_022edb3c;

typedef struct UnkStruct_022edb3c {
    u8 pad0[0x64];
    UnkSpr_022edb3c spr[21];
    s32 ids[21];
    u8 pad6a0[0x7e0 - 0x6a0];
    void *res0;
    void *res1;
    void *res2;
} UnkStruct_022edb3c;

void Records_Unk_ov001_022ed9f4(UnkSpr_022edb3c *spr, void *res, u16 idx, s32 prio, u16 x, u16 y);
void Records_Unk_ov001_022ed990(UnkSpr_022edb3c *spr, void *res, u16 idx, s32 prio, u16 x, u16 y);

#pragma thumb on
void Records_Unk_ov001_022edb3c(UnkStruct_022edb3c *p) {
    s32 i;
    void *res;
    UnkSpr_022edb3c *s;
    u16 z;

    res = p->res0;
    i = 0;
    s = &p->spr[0];
    z = 0;
    for (; i < 10; i++) {
        Records_Unk_ov001_022ed9f4(s, res, i, 2, z, z);
        s++;
    }
    res = p->res0;
    Records_Unk_ov001_022ed9f4(&p->spr[10], res, 10, 1, 0, 0);
    Records_Unk_ov001_022ed9f4(&p->spr[11], res, 13, 1, 0, 0);
    res = p->res0;
    Records_Unk_ov001_022ed9f4(&p->spr[12], res, 16, 1, 0, 4);
    res = p->res0;
    Records_Unk_ov001_022ed9f4(&p->spr[13], res, 17, 1, 0, 0);
    Records_Unk_ov001_022ed9f4(&p->spr[15], res, 19, 1, 0, 2);
    Records_Unk_ov001_022ed9f4(&p->spr[14], res, 21, 1, 0, 0);
    res = p->res1;
    Records_Unk_ov001_022ed9f4(&p->spr[16], res, 1, 1, 0, 0);
    res = p->res1;
    Records_Unk_ov001_022ed9f4(&p->spr[17], res, 2, 1, 0, 0);
    res = p->res1;
    Records_Unk_ov001_022ed9f4(&p->spr[18], res, 3, 1, 0, 0);
    res = p->res1;
    Records_Unk_ov001_022ed990(&p->spr[19], res, 0, 2, 0, 0);
    res = p->res2;
    Records_Unk_ov001_022ed9f4(&p->spr[20], res, 0, 3, 0, 0);
    for (i = 0; i < 21; i++)
        p->ids[i] = -1;
}
#pragma thumb reset
