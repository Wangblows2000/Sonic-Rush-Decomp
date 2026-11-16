#include <ov000_d/menuo.h>

#pragma thumb on

void func_ov000_02360bd4(UnkStruct_MenuO *w) {
    UnkStruct_MenuOSpr *spr;
    s32 *cur;

    spr = &w->spr[12];
    cur = &w->anim[12];
    if (*cur != 10) {
        func_02033550(spr, 10);
        *cur = 10;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[13];
    cur = &w->anim[13];
    if (*cur != 11) {
        func_02033550(spr, 11);
        *cur = 11;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[14];
    cur = &w->anim[14];
    if (*cur != 12) {
        func_02033550(spr, 12);
        *cur = 12;
    }
    spr->unk2C &= ~4;
    spr = &w->spr[15];
    cur = &w->anim[15];
    if (*cur != 13) {
        func_02033550(spr, 13);
        *cur = 13;
    }
    spr->unk2C &= ~4;
}

#pragma thumb reset
