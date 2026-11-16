/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208926c[];
extern u8 data_02087d44[];
void ClearDemo_Unk_02045cd0();

void ClearDemo_Unk_02045d10(s32 p0) {
    s32 t0 = *(s16 *)((u8 *)data_0208926c + 8);
    if (t0 < 0) {
        *(u16 *)((u8 *)data_0208926c + 0x8) = (t0 + 0x1);
        *(u16 *)((u8 *)data_02087d44 + 0x8) = (*(s16 *)((u8 *)data_02087d44 + 8) + 0x1);
    } else {
        if (t0 > 0) {
            *(u16 *)((u8 *)data_0208926c + 0x8) = (t0 - 0x1);
            *(u16 *)((u8 *)data_02087d44 + 0x8) = (*(s16 *)((u8 *)data_02087d44 + 8) - 0x1);
        }
    }
    if (*(s16 *)((u8 *)data_0208926c + 8) == 0) {
        *(u32 *)((u8 *)p0 + 0x340) = (u32)ClearDemo_Unk_02045cd0;
        *(u16 *)((u8 *)p0 + 0x162c) = 0;
    }
}

#pragma thumb reset
