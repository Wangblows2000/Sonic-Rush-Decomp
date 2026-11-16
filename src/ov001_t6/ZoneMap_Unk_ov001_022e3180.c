#include <nitro/types.h>

typedef struct {
    s32 id;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    s32 unk10;
    s32 unk14;
} UnkBgm_022e3180;

void NaviMsg_ShowMessage(s32 track, s32 a);
void NaviMsg_SetFaceAnim(s32 mode);
void NaviMsg_FadeVisible(void);
void NaviMsg_SetVisible(void);

#pragma thumb on
#pragma opt_propagation off
void ZoneMap_Unk_ov001_022e3180(UnkBgm_022e3180 *b, s32 id) {
    s32 mode;
    s32 time;
    BOOL fade;
    s32 track;
    BOOL play;

    if (b->id == id) {
        return;
    }
    b->id = id;
    mode = 0;
    b->unk8 = 0;
    time = 0xb4;
    b->unkc = time;
    b->unk10 = 3;
    play = TRUE;
    fade = TRUE;
    switch (id) {
    case 0:
        fade = FALSE;
        b->unkc = 0;
        play = FALSE;
        break;
    case 1:
        fade = FALSE;
        b->unkc = 0x3c;
        break;
    case 2:
        fade = FALSE;
        b->unkc = 0;
        play = FALSE;
        break;
    case 3:
        fade = FALSE;
        b->unkc = 0;
        break;
    case 4:
        track = 7;
        mode = 4;
        break;
    case 5:
        track = 8;
        mode = 4;
        break;
    case 6:
        track = 9;
        mode = 4;
        break;
    case 7:
        track = 10;
        mode = 4;
        break;
    case 8:
        track = 11;
        mode = 4;
        break;
    case 9:
        track = 12;
        mode = 4;
        break;
    case 10:
        track = 13;
        mode = 4;
        break;
    case 11:
        track = 14;
        mode = 4;
        break;
    case 25:
        track = 0;
        mode = 3;
        break;
    case 26:
        track = 2;
        mode = 2;
        break;
    case 27:
        track = 3;
        mode = 2;
        break;
    case 28:
        track = 4;
        mode = 6;
        break;
    case 29:
        track = 5;
        mode = 7;
        break;
    case 30:
        track = 6;
        mode = 8;
        break;
    default:
        switch (b->unk14) {
        case 0:
            switch (id) {
            case 12:
                track = 0x14;
                mode = 4;
                break;
            case 13:
                track = 0x15;
                mode = 4;
                break;
            case 14:
                track = 0x16;
                mode = 4;
                break;
            case 15:
                track = 0x17;
                mode = 4;
                break;
            case 23:
                track = 1;
                mode = 5;
                break;
            case 17:
                track = 0x18;
                mode = 4;
                break;
            case 18:
                track = 0x19;
                mode = 4;
                break;
            case 24:
                track = 0x1a;
                mode = 0;
                break;
            case 16:
            case 19:
            case 20:
            case 21:
            case 22:
                return;
            default:
                return;
            }
            break;
        case 1:
            switch (id) {
            case 14:
                track = 0x15;
                mode = 4;
                break;
            case 15:
                track = 0x16;
                mode = 4;
                break;
            case 23:
                track = 1;
                mode = 5;
                break;
            case 16:
                track = 0x14;
                mode = 4;
                break;
            case 19:
                track = 0x17;
                mode = 4;
                break;
            case 20:
                track = 0x18;
                mode = 4;
                break;
            case 21:
                track = 0x19;
                mode = 4;
                break;
            case 22:
                track = 0x1a;
                mode = 4;
                break;
            case 24:
                track = 0x1b;
                mode = 0;
                break;
            case 17:
            case 18:
                return;
            default:
                return;
            }
            break;
        }
        break;
    }
    if (play) {
        if (fade) {
            NaviMsg_ShowMessage(track, time);
        }
        NaviMsg_SetFaceAnim(mode);
        if (b->unk4 == 0) {
            b->unk4 = 1;
            if (id == 1) {
                NaviMsg_FadeVisible();
            } else {
                NaviMsg_SetVisible();
            }
        }
    } else if (b->unk4 != 0) {
        b->unk4 = 0;
        if (id == 2) {
            NaviMsg_FadeVisible();
        } else {
            NaviMsg_SetVisible();
        }
    }
}
#pragma thumb reset
