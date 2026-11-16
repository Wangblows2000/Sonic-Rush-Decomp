/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Boss7_Unk_ov000_022ec16c();
extern u8 gGameState[];
s32 Boss7PartD7_Action_Unk_ov000_022ec15c();
s32 BossEffect_CreateBoost();
extern u8 data_0206f470[];
s32 MTX_RotY33_();
extern u8 data_02071470[];
s32 Sound_PlaySeqArc();
void Boss7_State_Unk_ov000_022ed78c();

void Boss7_State_Unk_ov000_022ed8b4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x214);
    s32 t1 = *(u32 *)gPlayers;
    s32 t2 = Boss7_Unk_ov000_022ec16c(*(u32 *)((u8 *)p0 + 0x214), *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x214) + 0x3d0), gPlayers);
    *(u32 *)((u8 *)p0 + 0x218) = t2;
    s32 t3 = Boss7_Unk_ov000_022ec16c(t1, *(u32 *)gGameState);
    *(u32 *)((u8 *)p0 + 0x21c) = t3;
    Boss7PartD7_Action_Unk_ov000_022ec15c(*(u32 *)((u8 *)p0 + 0x218));
    Boss7PartD7_Action_Unk_ov000_022ec15c(*(u32 *)((u8 *)p0 + 0x21c));
    s32 t4 = BossEffect_CreateBoost(1, *(u32 *)((u8 *)t1 + 0x10), *(u32 *)((u8 *)t1 + 0x14), *(u32 *)((u8 *)t1 + 0x18));
    MTX_RotY33_((t4 + 0x158), *(s16 *)data_0206f470, *(s16 *)((u8 *)data_0206f470 + 0x2));
    s32 t5 = BossEffect_CreateBoost(1, *(u32 *)((u8 *)t0 + 0x10), *(u32 *)((u8 *)t0 + 0x14), *(u32 *)((u8 *)t0 + 0x18));
    MTX_RotY33_((t5 + 0x158), *(s16 *)data_02071470, *(s16 *)((u8 *)data_02071470 + 0x2));
    if (*(u8 *)((u8 *)t1 + 0x324) == 0) {
        Sound_PlaySeqArc(0, 0, 0xa);
    } else {
        Sound_PlaySeqArc(0, 0, 0x17);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss7_State_Unk_ov000_022ed78c;
}
