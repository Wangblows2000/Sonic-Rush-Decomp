/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_StartBgShake();
s32 Sound_PlaySeqArc();

void ExtraBoss_Unk_ov001_022eed0c(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)p0;
    if (*(u32 *)((u8 *)*(u32 *)p1 + 0x4) != 0) {
        return;
    }
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x160) | 0x200);
    *(u32 *)((u8 *)t0 + 0x160) = t1;
    ExtraBoss_StartBgShake(0x3c, 4, t1, t0);
    Sound_PlaySeqArc(0, 0, 0xb6);
}
