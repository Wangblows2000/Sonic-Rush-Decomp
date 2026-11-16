#include <nitro/types.h>

void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);

#pragma thumb on
void OptionsMenu_ShowMsgBox7(void) {
    MsgBox_Create(7, 3, 1, 1, 2, 0, 2, 0x200010);
}
#pragma thumb reset
