/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();
s32 ExtraBoss_Unk_ov001_02305648();
s32 ExtraBoss_Unk_ov001_02305564();

void ExtraBoss_Unk_ov001_023057f4(void) {
    s32 t0 = ExtraBoss_GetWork();
    s32 t1 = *(u32 *)((u8 *)t0 + 0x318);
    if (t1 == 0) {
        ExtraBoss_Unk_ov001_02305648(t1);
        return;
    }
    ExtraBoss_Unk_ov001_02305564();
}
