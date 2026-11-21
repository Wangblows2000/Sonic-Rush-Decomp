/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208926c[];
extern u8 data_02087d44[];
s32 StaffRoll_DrawSprites();
s32 _fflt();
s32 _fdiv();
s32 _fgr();
s32 StaffRoll_FadePalette();
s32 StaffRoll_CreateCreditTask();
extern u8 data_ov001_023160e4[];
s32 StaffRoll_BlackOutPicturePalette();
void StaffRoll_State_EndPictureHold();
s32 StaffRoll_SetState();

s32 StaffRoll_State_ShowEndPicture(u8 *p0) {
    s32 t0 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1a00);
    *(volatile u32 *)0x4001000 = t0;
    *(u16 *)((u8 *)data_0208926c + 0x8) = 0;
    *(u16 *)((u8 *)data_02087d44 + 0x8) = 0;
    StaffRoll_DrawSprites(p0, data_02087d44, 0, t0);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x14);
    if (t1 <= 0x40) {
        s32 t2 = _fflt(t1);
        s32 t3 = _fdiv(t2, 0x42800000);
        s32 t4 = _fgr(t3, 0x3f800000);
        s32 v5 = t3;
        if (t4 != 0) {
            v5 = 0x3f800000;
        }
        StaffRoll_FadePalette(((u8 *)p0 + 0xd4), 0x100, v5, ((u8 *)p0 + 0xf0c), ((u8 *)p0 + 0xd0c));
    }
    s32 t6 = *(u32 *)((u8 *)p0 + 0x14);
    s32 v7 = t6;
    if (t6 > 0x5e) {
        StaffRoll_CreateCreditTask(3);
        StaffRoll_BlackOutPicturePalette(((u8 *)p0 + 0xb34), *(u32 *)((u8 *)p0 + 0x41c), *(u16 *)((u8 *)data_ov001_023160e4 + 0x6), 0x100, ((u8 *)p0 + 0xf0c), ((u8 *)p0 + 0xd0c));
        s32 t8 = StaffRoll_SetState(p0, (u32)StaffRoll_State_EndPictureHold);
        v7 = t8;
    }
    return v7;
}

#pragma thumb reset
