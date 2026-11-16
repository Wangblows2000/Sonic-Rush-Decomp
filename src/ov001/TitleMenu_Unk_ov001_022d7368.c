#include <nitro/types.h>

void TitleMenu_Unk_ov001_022d7454(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);

#pragma thumb on
void TitleMenu_Unk_ov001_022d7368(void) {
    TitleMenu_Unk_ov001_022d7454();
    MsgBox_Create(0, 0, 1, 1, 2, 0, 0xe, 0x200010);
}
#pragma thumb reset
