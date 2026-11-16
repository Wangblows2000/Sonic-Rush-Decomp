#include <ov000_c/stageobj.h>

void func_ov000_0230b320(UnkStruct_StageObj *w);
void func_0201c070(void *a, s32 b);
void func_0201bd80(void *a, s32 b);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void GoalRing_CreatePart_ov000_0230a864(UnkStruct_StageObj *par, s16 dx, s16 dy, s16 vx, s16 vy);

void GoalRing_State_Unk_ov000_0230aed8(UnkStruct_StageObj *w) {
    w->unk8++;
    if (w->unk8 == 0x30)
        w->unk1F4 |= 1;
    if (w->unk1F4 & 1) {
        w->unkC--;
        if (w->unkC < 0x10)
            w->unkC = 0x10;
        if (w->unkC == 0x10 && (w->unk5C & 8)) {
            func_ov000_0230b320(w);
            w->unkC = 0x60;
            StageObj_SetAnimation(w, 7);
            func_0201c070((void *)w->unk1DC, 0);
            func_0201bd80((void *)w->unk1DC, 0x100);
            Sound_PlaySeqArcEx(0, 0, 0x46);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x1d0, 0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0, 0x1d0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x1d0, 0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0, -0x1d0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x148, 0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x148, -0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x148, -0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x148, 0x148);
            return;
        }
    }
    w->spr0.unk2A = w->unkC;
    w->spr1.unk2A = w->spr0.unk2A;
    if (w->unkC <= 0x40)
        func_0201bd80((void *)w->unk1DC, w->unkC << 2);
    if (!(w->unk8 & 0xf))
        GoalRing_CreatePart_ov000_0230a864(w, 0xf - (Rand() & 0x1f), 0xf - (Rand() & 0x1f), 0, 0);
}
