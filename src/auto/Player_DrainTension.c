/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0232ecac();
s32 EffVsTensionDrain_Create();
s32 Player_AddTension();
s32 Hud_SetTensionGauge();

void Player_DrainTension(u8 *p0) {
    if (*(u8 *)((u8 *)p0 + 0x325) == 0) {
        func_ov000_0232ecac(0x32, 0x80);
    }
    EffVsTensionDrain_Create(p0);
    Player_AddTension(p0, -0x12c0);
    Hud_SetTensionGauge(((u32)(*(s16 *)((u8 *)p0 + 0x350) << 0xc) >> 0x10), 1);
}
