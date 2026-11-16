#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02361e28_Sync;

UnkStruct_02361e28_Sync *func_020514e4(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL KeyframeAnim_IsFinished(UnkStruct_MenuOPos *p);
void func_ov000_02360a28(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360ad4(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360ab4(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360a94(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360a4c(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void func_ov000_02360a04(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_0236076c(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360974(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360950(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360870(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void func_ov000_02360790(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void TimeSelectMenu_DrawPlayerName(UnkStruct_MenuO *w, s32 x, s32 y, s32 row);
void func_ov000_02360748(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_02360724(UnkStruct_MenuO *w, s32 x, s32 y);
void func_ov000_023606c4(UnkStruct_MenuO *w, s32 x, s32 y, s32 scale);
void func_ov000_02360268(UnkStruct_MenuO *w);
void func_ov000_0236011c(UnkStruct_MenuO *w, s32 x, s32 y, BOOL flip);
void func_ov000_0235ffd0(UnkStruct_MenuO *w, s32 x, s32 y, BOOL flip);
void func_ov000_02360264(UnkStruct_MenuO *w);
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
    func_ov000_02360a28(w, p->x, p->y);
    if (w->unk4) {
        p = &w->pos[2];
        func_ov000_02360ad4(w, p->x, p->y);
    }
    p = &w->pos[3];
    func_ov000_02360ab4(w, p->x, p->y);
    p = &w->pos[4];
    func_ov000_02360a94(w, p->x, p->y);
    p = &w->pos[0];
    func_ov000_02360a4c(w, p->x, p->y, p->unk4);
    if (w->unk4) {
        p = &w->pos[5];
        func_ov000_02360a04(w, p->x, p->y);
    }
    p = &w->pos[8];
    func_ov000_0236076c(w, p->x, p->y);
    p = &w->pos[9];
    func_ov000_0236076c(w, p->x, p->y);
    if (w->unk28 == 0) {
        p = &w->pos[10];
        func_ov000_02360974(w, p->x, p->y);
        p = &w->pos[11];
        func_ov000_02360950(w, p->x, p->y);
        p = &w->pos[16];
        func_ov000_02360870(w, p->x, p->y, p->unk4);
        p = &w->pos[17];
        func_ov000_02360790(w, p->x, p->y, p->unk4);
    } else {
        p = &w->pos[10];
        func_ov000_02360950(w, p->x, p->y);
        p = &w->pos[11];
        func_ov000_02360974(w, p->x, p->y);
        p = &w->pos[16];
        func_ov000_02360790(w, p->x, p->y, p->unk4);
        p = &w->pos[17];
        func_ov000_02360870(w, p->x, p->y, p->unk4);
    }
    p = &w->pos[12];
    TimeSelectMenu_DrawPlayerName(w, p->x, p->y, 0);
    func_ov000_02360748(w, p->x, p->y);
    p = &w->pos[13];
    TimeSelectMenu_DrawPlayerName(w, p->x, p->y, 1);
    func_ov000_02360748(w, p->x, p->y);
    p = &w->pos[14];
    func_ov000_02360724(w, p->x, p->y);
    p = &w->pos[15];
    func_ov000_023606c4(w, p->x, p->y, p->unk4);
    func_ov000_02360268(w);
    if (w->unk28 == 0) {
        p = &w->pos[6];
        func_ov000_0236011c(w, p->x, p->y, TRUE);
        p = &w->pos[7];
        func_ov000_0235ffd0(w, p->x, p->y, FALSE);
    } else {
        p = &w->pos[6];
        func_ov000_0235ffd0(w, p->x, p->y, TRUE);
        p = &w->pos[7];
        func_ov000_0236011c(w, p->x, p->y, FALSE);
    }
    func_ov000_02360264(w);
    w->unk0++;
    if (done) {
        func_020514e4()->unk4 = 0;
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
