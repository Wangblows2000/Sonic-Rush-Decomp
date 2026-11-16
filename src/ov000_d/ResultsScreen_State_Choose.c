#include <ov000_d/sprobj.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkStruct_0235e79c_Pad;

extern UnkStruct_0235e79c_Pad gPad;

BOOL ResultsScreen_IsTopButtonTouched(void);
BOOL ResultsScreen_IsMiddleButtonTouched(void);
BOOL ResultsScreen_IsBottomButtonTouched(void);
void ResultsScreen_SetLocalChoice(UnkStruct_SprObj *w, s32 sel);
u8 *func_02051474(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void SysSound_PlaySE(s32 a);
void ResultsScreen_SetLocalDecided(UnkStruct_SprObj *w);
void func_ov000_0235e738(void);

#pragma thumb on

void ResultsScreen_State_Choose(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;
    BOOL done;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    func_ov000_0235def0(w, w->unk20, 0x1000);
    func_ov000_0235ddec(w, FALSE);
    done = FALSE;
    if (ResultsScreen_IsTopButtonTouched()) {
        w->unk8 = done;
        done = TRUE;
    } else if (ResultsScreen_IsMiddleButtonTouched()) {
        done = TRUE;
        w->unk8 = done;
    } else if (ResultsScreen_IsBottomButtonTouched()) {
        w->unk8 = 2;
        done = TRUE;
    } else if (gPad.trg & 1) {
        done = TRUE;
    } else if (gPad.rep & 0x40) {
        w->unk8--;
        if (w->unk8 < 0) {
            w->unk8 = 2;
        }
        w->unk4 = 0;
    } else if (gPad.rep & 0x80) {
        w->unk8++;
        if (w->unk8 >= 3) {
            w->unk8 = done;
        }
        w->unk4 = 0;
    }
    ResultsScreen_SetLocalChoice(w, w->unk8);
    w->unk0++;
    w->unk4++;
    if (done) {
        MI_CpuCopy8(func_02051474() + 6, w->unkC14, 6);
        SysSound_PlaySE(7);
        w->unk0 = 0;
        ResultsScreen_SetLocalDecided(w);
        gTaskCurrent->unk8 = func_ov000_0235e738;
    }
}

#pragma thumb reset
