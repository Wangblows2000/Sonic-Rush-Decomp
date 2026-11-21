#include <nitro/types.h>

typedef struct UnkWork_02059ffc {
    void (*fn)(struct UnkWork_02059ffc *w);
    u8 pad4[0x10];
    s32 unk14;
} UnkWork_02059ffc;

extern s32 data_0207c600[];

void SaveMsg_InitDisplay(UnkWork_02059ffc *w);
void MsgBox_Create(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void SaveMsg_State_CommitDelay(UnkWork_02059ffc *w);
void SaveMsg_State_FormatDelay(UnkWork_02059ffc *w);
void SaveMsg_State_Halt(UnkWork_02059ffc *w);

void SaveMsg_State_Init(UnkWork_02059ffc *w) {
    s32 id;

    SaveMsg_InitDisplay(w);
    *(vu16 *)0x04000304 &= ~0x8000;
    id = data_0207c600[w->unk14];
    if (w->unk14 == 3)
        id = 0x1a;
    MsgBox_Create(id, 3, 5, 0, 1, 1, 0, 0x100010);
    switch (w->unk14) {
    case 0:
        w->fn = SaveMsg_State_CommitDelay;
        break;
    case 3:
        w->fn = SaveMsg_State_FormatDelay;
        break;
    case 1:
    case 2:
    default:
        w->fn = SaveMsg_State_Halt;
        break;
    }
}
