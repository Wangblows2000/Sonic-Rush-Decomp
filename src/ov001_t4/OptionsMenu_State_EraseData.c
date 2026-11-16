#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
} UnkInfo_022cce10;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[8];
    s32 unk28;
    s32 unk2c;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3ac - 0x38];
    UnkInfo_022cce10 *info;
} UnkWork_022cce10;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cce10 *work;
} UnkTask_022cce10;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022cce10;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cce10;

typedef struct {
    u8 pad0[8];
    s16 brightness;
} UnkBr_022cce10;

extern UnkTask_022cce10 *gTaskCurrent;
extern UnkPad_022cce10 gPad;
extern UnkTouch_022cce10 gTouchState;
extern UnkBr_022cce10 data_0208926c;
extern UnkBr_022cce10 data_02087d44;

void OptionsMenu_UpdateAnims(UnkWork_022cce10 *w);
void OptionsMenu_DrawItems(UnkWork_022cce10 *w);
void OptionsMenu_Unk_ov001_022cdd28(void);
BOOL OptionsMenu_Unk_ov001_022c8ba8(u16 x, u16 y);
void SysSound_PlaySE(s32 a);
void Options_EraseSaveData(UnkInfo_022cce10 *info);
void MsgBox_Destroy(void);
void Task_Destroy(UnkTask_022cce10 *t);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);

#pragma thumb on
void OptionsMenu_State_EraseData(void) {
    UnkWork_022cce10 *w;
    s32 v;
    s32 limit;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->unk14++;
    w->unk18++;
    if (w->unk28 >= 0 && w->unk2c == 1 && w->unk30 == 0) {
        if (w->unk28 == 0x1f) {
            SysSound_PlaySE(3);
        }
        if (w->unk28 == 0x20) {
            Options_EraseSaveData(w->info);
        }
        w->unk28++;
        if (w->unk28 >= 0x40) {
            s16 b;
            v = w->unk28 - 0x40;
            b = v / 2;
            data_0208926c.brightness = b;
            data_02087d44.brightness = b;
            if (v >= 0x20) {
                MsgBox_Destroy();
                w->info->unk0 = 10;
                w->info->unk4 = 4;
                Task_Destroy(gTaskCurrent);
                return;
            }
        }
    }
    if (w->unk28 >= 0) {
        return;
    }
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            OptionsMenu_Unk_ov001_022cdd28();
        }
    }
    if (w->unk1c >= 0) {
        return;
    }
    if (w->unk34 != -1) {
        if (w->unk2c == 0 && w->unk30 == 0) {
            limit = 0x20;
        } else {
            limit = 0x10;
        }
        if (w->unk18 >= limit) {
            switch (w->unk2c) {
            case 0:
                switch (w->unk30) {
                case 0:
                    w->unk2c = 1;
                    w->unk30 = 1;
                    break;
                case 1:
                    OptionsMenu_Unk_ov001_022cdd28();
                    break;
                }
                break;
            case 1:
                switch (w->unk30) {
                case 0:
                    MsgBox_Create(0x1a, 3, 5, 1, 2, 0, 2, 0x200010);
                    w->unk28 = 0;
                    break;
                case 1:
                    OptionsMenu_Unk_ov001_022cdd28();
                    break;
                }
                break;
            }
            w->unk34 = -1;
            w->unk18 = 0;
        }
        return;
    }
    if (gPad.rep & 0x20) {
        if (w->unk30 != 0) {
            w->unk30 = 0;
            w->unk14 = 0;
            SysSound_PlaySE(4);
        }
    } else if (gPad.rep & 0x10) {
        if (w->unk30 != 1) {
            w->unk30 = 1;
            w->unk14 = 0;
            SysSound_PlaySE(4);
        }
    }
    if (gPad.trg & 1) {
        w->unk34 = w->unk30;
        w->unk18 = 0;
        SysSound_PlaySE(0);
        return;
    }
    if (gPad.trg & 2) {
        SysSound_PlaySE(1);
        OptionsMenu_Unk_ov001_022cdd28();
        return;
    }
    if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x < 0x100 && y < 0xc0) {
            if (OptionsMenu_Unk_ov001_022c8ba8(x, y)) {
                w->unk1c = 0;
                SysSound_PlaySE(1);
            }
            if (x >= 0x20 && x < 0x7a && y >= 0x50 && y < 0x70) {
                w->unk30 = 0;
                w->unk34 = w->unk30;
                w->unk18 = 0;
                SysSound_PlaySE(0);
            }
            if (x >= 0x88 && x < 0xe2 && y >= 0x50 && y < 0x70) {
                w->unk30 = 1;
                w->unk34 = w->unk30;
                w->unk18 = 0;
                SysSound_PlaySE(0);
            }
        }
    }
}
#pragma thumb reset
