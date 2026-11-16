/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gZoneActSelectWork[];
s32 NaviMsg_ShowMessage();
s32 NaviMsg_SetFaceAnim();

void ZoneActSelect_ShowIntroNaviMessage(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gZoneActSelectWork + 0x10);
    if ((*(u32 *)((u8 *)*(u32 *)gZoneActSelectWork + 0x10) & 2) == 0) {
        if ((t0 & 4) == 0) {
            NaviMsg_ShowMessage(0, 0xb4, (t0 & 4));
            NaviMsg_SetFaceAnim(3);
            return;
        }
        NaviMsg_ShowMessage(3, 0xb4);
        NaviMsg_SetFaceAnim(2);
    }
}

#pragma thumb reset
