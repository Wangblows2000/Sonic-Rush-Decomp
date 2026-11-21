/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss2Water_State_LowerShake();
s32 ScreenShake_Start();
s32 Sound_PlaySeqArc();

void Boss2Water_Action_StartLower(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x210) + 0x10);
    *(u32 *)((u8 *)t0 + 0x4) = (u32)Boss2Water_State_LowerShake;
    ScreenShake_Start(0xc, (u32)Boss2Water_State_LowerShake);
    *(u32 *)((u8 *)t0 + 0xc) = 0xf;
    Sound_PlaySeqArc(*(u32 *)((u8 *)p0 + 0x1454), 0, 0x9f);
}
