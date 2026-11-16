/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBossStatus[];
void Boss7PartD5_InputState_Unk_ov000_022ea798();
void Boss7PartD5_InputState_Unk_ov000_022ea190();

s32 Boss7PartD5_InputState_Unk_ov000_022ea1b8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)gBossStatus + 0x4);
    if (t0 != 0x27) {
        *(u32 *)((u8 *)p0 + 0x43c) = (u32)Boss7PartD5_InputState_Unk_ov000_022ea798;
        return 0;
    }
    *(u32 *)((u8 *)p0 + 0x43c) = (u32)Boss7PartD5_InputState_Unk_ov000_022ea190;
    s32 t1 = ((s32 (*)())*(u32 *)((u8 *)p0 + 0x43c))(p0, *(u32 *)((u8 *)p0 + 0x43c));
    return t1;
}
