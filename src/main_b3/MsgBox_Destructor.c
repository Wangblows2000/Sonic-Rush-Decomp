#include <nitro/types.h>

typedef struct { s32 unk0; u8 pad4[0x30]; void *unk34; u8 pad38[0x10]; } UnkSpr_0204fe60;
typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x1c];
    s32 unk24;
    u8 pad28[0x24];
    s32 unk4C;
    u8 pad50[0x350];
    UnkSpr_0204fe60 spr[5]; /* 0x3a0 */
} UnkWork_0204fe60;
typedef struct { u8 pad0[0x10]; UnkWork_0204fe60 *work; } UnkTask_0204fe60;
extern void *gMsgBoxTask;
extern s32 gMsgBoxResult;

void MsgBox_Unk_0204efa8(UnkWork_0204fe60 *w);
void SysSound_Release(void);
void ObjVram_Free(s32 a, void *b);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MsgBox_Unk_0204f454(UnkWork_0204fe60 *w, s32 i);
void MsgBox_Unk_0204f2fc(UnkWork_0204fe60 *w, s32 a);

#pragma thumb on
void MsgBox_Destructor(UnkTask_0204fe60 *t) {
    UnkWork_0204fe60 *w = t->work;
    s32 i;

    MsgBox_Unk_0204efa8(w);
    if (w->unk4C)
        SysSound_Release();
    for (i = 0; i < 5; i++) {
        vu16 zero;
        if (w->spr[i].unk34 != NULL)
            ObjVram_Free(w->spr[i].unk0, w->spr[i].unk34);
        zero = 0;
        MIi_CpuClear16(zero, &w->spr[i], sizeof(UnkSpr_0204fe60));
    }
    for (i = 0; i < 0xd; i++)
        MsgBox_Unk_0204f454(w, i);
    MsgBox_Unk_0204f2fc(w, 0);
    gMsgBoxTask = NULL;
    switch (w->unk24) {
    case 0:
        gMsgBoxResult = 0;
        break;
    case 1:
        if (w->unk4 == 0)
            gMsgBoxResult = 0;
        else
            gMsgBoxResult = 1;
        break;
    case 2:
        gMsgBoxResult = 2;
        break;
    case 3:
    default:
        gMsgBoxResult = 4;
        break;
    }
}
#pragma thumb reset
