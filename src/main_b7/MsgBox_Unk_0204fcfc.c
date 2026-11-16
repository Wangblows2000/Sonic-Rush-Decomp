#include <nitro/types.h>

typedef struct {
    u32 timer;
    u8 pad4[0x4c];
    s32 perFrame;
    u8 pad54[0x51c - 0x54];
    s32 need[13];
    void *files[13];
    s32 cur;
} UnkWork_0204fcfc;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204fcfc *work;
} UnkTask_0204fcfc;

extern UnkTask_0204fcfc *gTaskCurrent;
void MsgBox_Unk_0204f4d4(UnkWork_0204fcfc *w, s32 idx);
void MsgBox_Unk_0204fe20(void);

#pragma thumb on
void MsgBox_Unk_0204fcfc(void) {
    UnkWork_0204fcfc *w = gTaskCurrent->work;
    s32 n;
    if (w->timer < 1) {
        w->timer++;
        return;
    }
    n = 0;
    if (w->perFrame > 0) {
        do {
            s32 i = w->cur;
            if (w->need[i] != 0) {
                MsgBox_Unk_0204f4d4(w, i);
                n++;
            } else {
                w->files[i] = NULL;
            }
            w->cur++;
        } while (w->cur < 0xd && n < w->perFrame);
    }
    w->timer++;
    if (w->cur >= 0xd)
        MsgBox_Unk_0204fe20();
}
#pragma thumb reset
