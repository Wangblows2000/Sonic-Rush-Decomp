#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x24];
    UnkStruct_Spr spr[18];
    s32 unk534[18];
    u8 pad57C[0xE14 - 0x57C];
    void *unkE14;
    void *unkE18;
    void *unkE1C;
    void *unkE20;
} UnkStruct_0235ee18;

void func_ov000_0235ecfc(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d);
void func_ov000_0235ec98(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d);

#pragma thumb on

void func_ov000_0235ee18(UnkStruct_0235ee18 *w) {
    void *res;
    s32 i;

    res = w->unkE1C;
    func_ov000_0235ecfc(&w->spr[0], res, 3, 0, 0, 0);
    res = w->unkE1C;
    func_ov000_0235ecfc(&w->spr[1], res, 4, 2, 0, 0);
    res = w->unkE1C;
    func_ov000_0235ecfc(&w->spr[2], res, 6, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[3], res, 7, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[4], res, 8, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[5], res, 9, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[6], res, 6, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[7], res, 7, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[8], res, 8, 2, 0, 4);
    func_ov000_0235ecfc(&w->spr[9], res, 9, 2, 0, 4);
    res = w->unkE18;
    func_ov000_0235ecfc(&w->spr[10], res, 0xb, 0, 0, 0);
    func_ov000_0235ecfc(&w->spr[11], res, 0xb, 0, 0, 0);
    res = w->unkE14;
    func_ov000_0235ec98(&w->spr[12], res, 0, 2, 0, 0);
    res = w->unkE18;
    func_ov000_0235ec98(&w->spr[13], res, 0, 2, 0, 4);
    func_ov000_0235ec98(&w->spr[14], res, 3, 2, 0, 4);
    func_ov000_0235ec98(&w->spr[15], res, 6, 2, 0, 4);
    func_ov000_0235ec98(&w->spr[16], w->unkE18, 0xc, 2, 0, 4);
    func_ov000_0235ec98(&w->spr[17], w->unkE20, 1, 0, 0xc, 0);
    w->spr[17].unk4 = 0xe0;
    w->spr[17].unk6 = 0xa0;
    for (i = 0; i < 18; i++)
        w->unk534[i] = -1;
}

#pragma thumb reset
