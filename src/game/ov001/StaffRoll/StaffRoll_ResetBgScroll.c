/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Task_Destroy();
extern u8 data_ov001_023160f8[];
extern u8 gDisplayMain[];
extern u8 data_0208922e[];
extern u8 gDisplaySub[];
extern u8 data_02087d06[];

void StaffRoll_ResetBgScroll(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 != 0) {
        Task_Destroy(t0);
        *(u32 *)((u8 *)p0 + 0x8) = 0;
    }
    s32 t1 = (*(u8 *)((u8 *)data_ov001_023160f8 + 0x4) << 2);
    *(u16 *)((u8 *)gDisplayMain + t1) = 0;
    *(u16 *)((u8 *)data_0208922e + t1) = 0;
    s32 t2 = (*(u8 *)((u8 *)data_ov001_023160f8 + 0x7) << 2);
    *(u16 *)((u8 *)gDisplaySub + t2) = 0;
    *(u16 *)((u8 *)data_02087d06 + t2) = 0;
}

#pragma thumb reset
