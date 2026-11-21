/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 StaffRoll_DrawSprites();
s32 _fflt();
s32 _fdiv();
s32 _fgr();
s32 _fsub();
s32 StaffRoll_FadePalette();
void StaffRoll_State_ExtraNextPicture();
s32 StaffRoll_SetState();

void StaffRoll_State_ExtraPictureFadeOut(u8 *p0) {
    StaffRoll_DrawSprites();
    s32 t0 = *(u32 *)((u8 *)p0 + 0x14);
    if (t0 <= 0x90) {
        s32 t1 = _fflt(t0);
        s32 t2 = _fdiv(t1, 0x43100000);
        s32 t3 = _fgr(t2, 0x3f800000);
        s32 v4 = t2;
        if (t3 != 0) {
            v4 = 0x3f800000;
        }
        s32 t5 = _fsub(0x3f800000, v4);
        StaffRoll_FadePalette(((s32)((u8 *)p0 + 0x54) + ((*(u32 *)((u8 *)p0 + 0xd08) + 0xb) << 6)), 0x100, t5, ((u8 *)p0 + 0xf0c), ((u8 *)p0 + 0xd0c));
        return;
    }
    StaffRoll_SetState(p0, (u32)StaffRoll_State_ExtraNextPicture);
}

#pragma thumb reset
