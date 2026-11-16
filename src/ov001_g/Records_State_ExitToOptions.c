#include <nitro/types.h>

typedef struct UnkWork_022ed4d8 {
    u32 timer;
    u8 pad4[0x20];
    s32 unk24;
} UnkWork_022ed4d8;

typedef struct UnkTask_022ed4d8 {
    u8 pad0[0x10];
    UnkWork_022ed4d8 *work;
} UnkTask_022ed4d8;

typedef struct UnkBright_022ed4d8 {
    u8 pad0[8];
    s16 unk8;
} UnkBright_022ed4d8;

extern UnkTask_022ed4d8 *gTaskCurrent;
extern UnkBright_022ed4d8 data_0208926c;
extern UnkBright_022ed4d8 data_02087d44;

void Records_Unk_ov001_022ed490(UnkWork_022ed4d8 *w, s16 x, s16 y);
void Records_Unk_ov001_022ed3e0(UnkWork_022ed4d8 *w);
void Records_DrawBackButton(UnkWork_022ed4d8 *w, s32 flag);
void Records_Unk_ov001_022ed19c(UnkWork_022ed4d8 *w);
void Records_Unk_ov001_022ed188(UnkWork_022ed4d8 *w);
void Records_Unk_ov001_022ed14c(UnkWork_022ed4d8 *w);
void Records_Unk_ov001_022ed104(UnkWork_022ed4d8 *w);
void Records_Unk_ov001_022ed08c(UnkWork_022ed4d8 *w);
void Task_DestroyAll(void);
void Options_StartFromRecords(void);

#pragma thumb on
void Records_State_ExitToOptions(void) {
    UnkWork_022ed4d8 *w = gTaskCurrent->work;
    Records_Unk_ov001_022ed490(w, 0, 0x90);
    Records_Unk_ov001_022ed3e0(w);
    Records_DrawBackButton(w, w->unk24);
    Records_Unk_ov001_022ed19c(w);
    Records_Unk_ov001_022ed188(w);
    Records_Unk_ov001_022ed14c(w);
    Records_Unk_ov001_022ed104(w);
    Records_Unk_ov001_022ed08c(w);
    w->timer++;
    if (w->timer >= 0x10) {
        data_0208926c.unk8 = -16;
        data_02087d44.unk8 = -16;
        Task_DestroyAll();
        Options_StartFromRecords();
    }
}
#pragma thumb reset
