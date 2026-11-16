/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 func_ov000_022ffa58();
extern u8 gSaveData[];
extern u8 data_022c4560[];
s32 func_ov000_023008a0();
s32 func_ov000_02300808();

void EnemyFlapperGroup_State_Unk_ov000_02300338(u8 *p0) {
    if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 0xc) {
        if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) == 0) {
            Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x78);
            Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
            func_ov000_022ffa58(p0, 0, 0);
        }
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x8) = 0x78;
        if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) != 0) {
            s32 t1 = *(u32 *)((u8 *)data_022c4560 + 0x10);
            if (t1 != 2) {
                if (t1 != 3) {
                    *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + ((u32)*(u32 *)((u8 *)p0 + 0x8) >> 1));
                }
            }
        }
        s32 t2 = *(u32 *)((u8 *)p0 + 0x1e0);
        if (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) == 0) {
            if (*(u8 *)((u8 *)t2 + 0x8) == 0) {
                func_ov000_023008a0(p0, t2);
                return;
            }
        }
        func_ov000_02300808(p0);
        return;
    }
    *(u32 *)((u8 *)p0 + 0x8) = (t0 - 0x1);
}
