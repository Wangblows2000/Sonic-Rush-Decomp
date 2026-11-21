#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    s32 unk4;
    s32 unk8;
    u8 unkc;
    u8 padd;
    u16 unke;
} UnkFade_0206199c;

void TalkDemo_ResetBg1Scroll(UnkFade_0206199c *f);
void TalkDemo_SetBg1Scroll(UnkFade_0206199c *f);
BOOL TalkDemo_State_ShakeBg1(UnkFade_0206199c *f);

#pragma thumb on
BOOL TalkDemo_StartBg1Effect(UnkFade_0206199c *f, u32 mode) {
    switch (mode) {
    case 0:
    case 2:
    default:
        TalkDemo_ResetBg1Scroll(f);
        f->state = NULL;
        break;
    case 1:
        if (f->unkc == 2) {
            f->unk4 = 0;
            f->unk8 = 0x3f00;
        } else {
            f->unk4 = 0x5000;
            f->unk8 = 0x3f00;
        }
        f->state = NULL;
        break;
    case 3:
        f->unke = 0x1e;
        f->state = (void (*)(void *))TalkDemo_State_ShakeBg1;
        TalkDemo_ResetBg1Scroll(f);
        break;
    }
    TalkDemo_SetBg1Scroll(f);
    return TRUE;
}
#pragma thumb reset
