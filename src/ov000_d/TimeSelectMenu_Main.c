#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02361e28_Sync;

UnkStruct_02361e28_Sync *WirelessSession_GetSendBuffer(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL KeyframeAnim_IsFinished(UnkStruct_MenuOPos *p);
void TimeSelectMenu_DrawSprite150(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite30(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite78(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSpriteC0(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite0Scaled(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void TimeSelectMenu_DrawSprite198(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite588(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite2B8(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite300(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSpriteGroupB(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void TimeSelectMenu_DrawSpriteGroupA(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void TimeSelectMenu_DrawPlayerName(UnkStruct_MenuO *w, s32 x, s32 y, s32 row);
void TimeSelectMenu_DrawSprite5D0(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSprite618(UnkStruct_MenuO *w, s32 x, s32 y);
void TimeSelectMenu_DrawSpriteScaled(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void TimeSelectMenu_SetupOrthoCamera(UnkStruct_MenuO *w);
void TimeSelectMenu_DrawModelB(UnkStruct_MenuO *w, s32 x, s32 y, BOOL flip);
void TimeSelectMenu_DrawModelA(UnkStruct_MenuO *w, s32 x, s32 y, BOOL flip);
void TimeSelectMenu_Unk_ov000_02360264(UnkStruct_MenuO *w);
void TimeSelectMenu_State_Select(void);
void TimeSelectMenu_State_FollowChooser(void);

#pragma thumb on

void TimeSelectMenu_Main(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    UnkStruct_MenuOPos *p;
    BOOL done;
    s32 i;

    if (w->unk0 == 2) {
        if (w->unk4) {
            MsgBox_Create(0xa, 3, 5, 1, 0, 0, 0xe, 0x200010);
            w->unk14 = 1;
        } else {
            MsgBox_Create(0xd, 3, 5, 1, 0, 0, 0xe, 0x200010);
            w->unk14 = 1;
        }
    }
    done = TRUE;
    for (i = 0, p = w->pos; i < 0x12; p++, i++) {
        KeyframeAnim_Update(p);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
    }
    p = &w->pos[1];
    TimeSelectMenu_DrawSprite150(w, p->x, p->y);
    if (w->unk4) {
        p = &w->pos[2];
        TimeSelectMenu_DrawSprite30(w, p->x, p->y);
    }
    p = &w->pos[3];
    TimeSelectMenu_DrawSprite78(w, p->x, p->y);
    p = &w->pos[4];
    TimeSelectMenu_DrawSpriteC0(w, p->x, p->y);
    p = &w->pos[0];
    TimeSelectMenu_DrawSprite0Scaled(w, p->x, p->y, p->unk4);
    if (w->unk4) {
        p = &w->pos[5];
        TimeSelectMenu_DrawSprite198(w, p->x, p->y);
    }
    p = &w->pos[8];
    TimeSelectMenu_DrawSprite588(w, p->x, p->y);
    p = &w->pos[9];
    TimeSelectMenu_DrawSprite588(w, p->x, p->y);
    if (w->unk28 == 0) {
        p = &w->pos[10];
        TimeSelectMenu_DrawSprite2B8(w, p->x, p->y);
        p = &w->pos[11];
        TimeSelectMenu_DrawSprite300(w, p->x, p->y);
        p = &w->pos[16];
        TimeSelectMenu_DrawSpriteGroupB(w, p->x, p->y, p->unk4);
        p = &w->pos[17];
        TimeSelectMenu_DrawSpriteGroupA(w, p->x, p->y, p->unk4);
    } else {
        p = &w->pos[10];
        TimeSelectMenu_DrawSprite300(w, p->x, p->y);
        p = &w->pos[11];
        TimeSelectMenu_DrawSprite2B8(w, p->x, p->y);
        p = &w->pos[16];
        TimeSelectMenu_DrawSpriteGroupA(w, p->x, p->y, p->unk4);
        p = &w->pos[17];
        TimeSelectMenu_DrawSpriteGroupB(w, p->x, p->y, p->unk4);
    }
    p = &w->pos[12];
    TimeSelectMenu_DrawPlayerName(w, p->x, p->y, 0);
    TimeSelectMenu_DrawSprite5D0(w, p->x, p->y);
    p = &w->pos[13];
    TimeSelectMenu_DrawPlayerName(w, p->x, p->y, 1);
    TimeSelectMenu_DrawSprite5D0(w, p->x, p->y);
    p = &w->pos[14];
    TimeSelectMenu_DrawSprite618(w, p->x, p->y);
    p = &w->pos[15];
    TimeSelectMenu_DrawSpriteScaled(w, p->x, p->y, p->unk4);
    TimeSelectMenu_SetupOrthoCamera(w);
    if (w->unk28 == 0) {
        p = &w->pos[6];
        TimeSelectMenu_DrawModelB(w, p->x, p->y, TRUE);
        p = &w->pos[7];
        TimeSelectMenu_DrawModelA(w, p->x, p->y, FALSE);
    } else {
        p = &w->pos[6];
        TimeSelectMenu_DrawModelA(w, p->x, p->y, TRUE);
        p = &w->pos[7];
        TimeSelectMenu_DrawModelB(w, p->x, p->y, FALSE);
    }
    TimeSelectMenu_Unk_ov000_02360264(w);
    w->unk0++;
    if (done) {
        WirelessSession_GetSendBuffer()->unk4 = 0;
        w->unk0 = 0;
        if (w->unk4) {
            w->unk18 = 0;
            gTaskCurrent->unk8 = TimeSelectMenu_State_Select;
            return;
        }
        KeyframeAnim_SetAnim(&w->pos[0], 6);
        w->unkC = 0;
        w->unk10 = 0;
        gTaskCurrent->unk8 = TimeSelectMenu_State_FollowChooser;
    }
}

#pragma thumb reset
