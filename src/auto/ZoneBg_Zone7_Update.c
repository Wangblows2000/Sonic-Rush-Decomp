/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 ZoneBg_Zone7_Unk_ov000_0233ea38();
s32 ZoneBg_Zone7_UpdateParallax();

void ZoneBg_Zone7_Update(s32 p0) {
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 1) != 0) {
        ZoneBg_Zone7_Unk_ov000_0233ea38(p0, 0);
        ZoneBg_Zone7_UpdateParallax(p0);
        return;
    }
    ZoneBg_Zone7_Unk_ov000_0233ea38(p0, 0);
    ZoneBg_Zone7_Unk_ov000_0233ea38(p0, 1);
}
