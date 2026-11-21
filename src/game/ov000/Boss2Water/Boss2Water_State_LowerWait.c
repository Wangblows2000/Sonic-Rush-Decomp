/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss2Water_State_Lower();
s32 ScreenShake_Start();

void Boss2Water_State_LowerWait(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x4) = (u32)Boss2Water_State_Lower;
    ScreenShake_Start(9, (u32)Boss2Water_State_Lower);
    *(u32 *)((u8 *)p0 + 0xc) = 0x78;
}
