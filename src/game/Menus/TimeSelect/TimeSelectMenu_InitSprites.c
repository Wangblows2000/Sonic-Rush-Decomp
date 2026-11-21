#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x30];
    UnkStruct_Spr spr[24];
    s32 unk6F0[24];
    u8 pad750[0x1010 - 0x750];
    void *unk1010;
    void *unk1014;
    void *unk1018;
    void *unk101C;
    void *unk1020;
    void *unk1024;
} UnkStruct_023626a4;

void TimeSelectMenu_InitSubSprite(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c);
void TimeSelectMenu_InitMainSprite(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c);

#pragma thumb on

void TimeSelectMenu_InitSprites(UnkStruct_023626a4 *w) {
    void *res;
    s32 i;

    res = w->unk1010;
    TimeSelectMenu_InitSubSprite(&w->spr[0], res, 0, 1, 8);
    TimeSelectMenu_InitSubSprite(&w->spr[1], res, 2, 1, 2);
    TimeSelectMenu_InitSubSprite(&w->spr[2], res, 2, 1, 2);
    TimeSelectMenu_InitSubSprite(&w->spr[3], res, 5, 1, 6);
    res = w->unk1014;
    TimeSelectMenu_InitSubSprite(&w->spr[4], res, 0, 1, 4);
    res = w->unk1020;
    TimeSelectMenu_InitSubSprite(&w->spr[5], res, 0, 1, 4);
    res = w->unk1018;
    TimeSelectMenu_InitSubSprite(&w->spr[6], res, 5, 1, 4);
    res = w->unk101C;
    TimeSelectMenu_InitSubSprite(&w->spr[7], res, 1, 1, 2);
    TimeSelectMenu_InitSubSprite(&w->spr[8], res, 2, 1, 2);
    res = w->unk1018;
    TimeSelectMenu_InitMainSprite(&w->spr[9], res, 0, 0, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[10], res, 1, 0, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[11], res, 6, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[12], res, 7, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[13], res, 8, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[14], res, 9, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[15], res, 0xa, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[16], res, 0xb, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[17], res, 0xc, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[18], res, 0xd, 1, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[19], res, 4, 1, 2);
    TimeSelectMenu_InitMainSprite(&w->spr[20], res, 3, 0, 4);
    TimeSelectMenu_InitMainSprite(&w->spr[21], res, 2, 0, 2);
    TimeSelectMenu_InitMainSprite(&w->spr[22], w->unk101C, 0, 0, 0);
    TimeSelectMenu_InitSubSprite(&w->spr[23], w->unk1024, 1, 0, 0);
    w->spr[23].unk44 = 0xc;
    w->spr[23].unk4 = 0xe0;
    w->spr[23].unk6 = 8;
    for (i = 0; i < 24; i++)
        w->unk6F0[i] = -1;
}

#pragma thumb reset
