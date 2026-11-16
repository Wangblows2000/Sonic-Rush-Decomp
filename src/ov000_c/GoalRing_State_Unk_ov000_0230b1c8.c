#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x24];
    u32 unk24;
} UnkStruct_022c4590;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4590 data_022c4590;

void GoalRing_CreatePart_ov000_0230a864(UnkStruct_StageObj *par, s16 dx, s16 dy, s16 vx, s16 vy);
void BrightnessFade_Create(s32 a, s32 b);
void StageShutdown_Create(void (*f)(void));
void ResultsScreen_Create(void);
void ClearDemo_Create(void);

void GoalRing_State_Unk_ov000_0230b1c8(UnkStruct_StageObj *w) {
    w->unk8++;
    *(s16 *)((u8 *)w + 0x148) = -300;
    *(s16 *)((u8 *)w + 0x14A) = 300;
    if (w->spr0.unk8 != 7)
        return;
    if (!(w->unk8 & 0xf))
        GoalRing_CreatePart_ov000_0230a864(w, 0xf - (Rand() & 0x1f), 0xf - (Rand() & 0x1f), 0, 0);
    if (w->unkC == 8)
        BrightnessFade_Create(4, 0x200);
    if (w->unkC == 0) {
        if (gGameState.unk10 == 1) {
            data_022c4590.unk24 &= ~1;
            data_022c4590.unk24 &= ~2;
            StageShutdown_Create(ResultsScreen_Create);
        } else {
            StageShutdown_Create(ClearDemo_Create);
        }
    } else {
        w->unkC--;
    }
}
