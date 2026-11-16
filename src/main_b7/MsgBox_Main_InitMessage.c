#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    s32 unk10;
    u8 pad14[8];
    s32 unk1c;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x3a0 - 0x28];
    u8 unk3a0[0x90];
    u8 unk430[0x48];
    u8 unk478[0x48];
    u8 pad4c0[0x550 - 0x4c0];
    void *files[2];
} UnkWork_0204fa74;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204fa74 *work;
} UnkTask_0204fa74;

extern UnkTask_0204fa74 *gTaskCurrent;
void MsgBox_InitSprite(UnkWork_0204fa74 *w, void *a, void *b, u16 idx, s32 d);
extern s32 data_02074ca8[];
extern u16 data_02074c04[];
void MsgBox_BeginOpen(void);

#pragma thumb on
void MsgBox_Main_InitMessage(void) {
    UnkWork_0204fa74 *w = gTaskCurrent->work;
    s32 st = w->unk1c;
    if (st != 0x1c) {
        void *b = w->files[data_02074ca8[st]];
        void *a = (u8 *)w + 0x3e8;
        s32 idx;
        if (st < 0x1b)
            idx = data_02074c04[st];
        else
            idx = -1;
        if (idx >= 0)
            MsgBox_InitSprite(w, a, b, idx, 0);
    }
    MsgBox_BeginOpen();
}
#pragma thumb reset
