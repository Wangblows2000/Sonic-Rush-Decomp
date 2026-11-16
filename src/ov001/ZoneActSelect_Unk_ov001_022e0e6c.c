#include <nitro/types.h>

extern u32 *gZoneActSelectWork;
void NaviMsg_ShowMessage(s32 a, s32 b);
void NaviMsg_SetFaceAnim(s32 a);

#pragma thumb on
void ZoneActSelect_Unk_ov001_022e0e6c(void) {
    if (!(gZoneActSelectWork[4] & 2)) {
        NaviMsg_ShowMessage(4, 0xb4);
        NaviMsg_SetFaceAnim(6);
    }
}
#pragma thumb reset
