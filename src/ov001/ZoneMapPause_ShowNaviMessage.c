#include <nitro/types.h>

void NaviMsg_ShowMessage(s32 a, s32 b);
void NaviMsg_SetFaceAnim(s32 a);

#pragma thumb on
void ZoneMapPause_ShowNaviMessage(void) {
    NaviMsg_ShowMessage(5, 0xb4);
    NaviMsg_SetFaceAnim(7);
}
#pragma thumb reset
