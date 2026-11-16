/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjRect_FuncNoHit();
s32 Sound_PlaySeqArcEx();
s32 EffZakoBombBurst_Create();
s32 Player_Action_AttackBounce();
s32 func_ov000_0230da90();
s32 ItemBox_CreatePart_ov000_0230d5d4();

void ItemBox_OnHit(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    s32 t2 = *(s16 *)((u8 *)(t1 + 0x300) + 0xc8);
    if (t2 != 0) {
        ObjRect_FuncNoHit(p0, p1, t2);
        return;
    }
    Sound_PlaySeqArcEx(0, 0, 0x34);
    EffZakoBombBurst_Create(t0, 0, 0);
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 8);
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 2);
    *(u32 *)((u8 *)t0 + 0x1f4) = (*(u32 *)((u8 *)t0 + 0x1f4) | 0x10);
    if (t1 != 0) {
        if (*(u8 *)t1 == 1) {
            Player_Action_AttackBounce(t1);
        }
    }
    func_ov000_0230da90(t0, t1, ((u32)(*(s8 *)((u8 *)*(u32 *)((u8 *)t0 + 0x1e0) + 0x6) << 0x10) >> 0x10));
    ItemBox_CreatePart_ov000_0230d5d4(t1, 0, 0, ((u32)(*(s8 *)((u8 *)*(u32 *)((u8 *)t0 + 0x1e0) + 0x6) << 0x10) >> 0x10));
}
