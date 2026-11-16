/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 ZoneFx_Zone2_StopVAlarms();
s32 ZoneFx_Zone2_InitVAlarms();

void ZoneFx_Zone2_Unk_ov000_0233d4bc(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)gCamera + 0xc8) & 0x2000000);
    if (t0 != 0) {
        s32 t1 = *(u32 *)p0;
        s32 t2 = (*(u32 *)p0 & 0x1000);
        if (t2 == 0) {
            *(u32 *)p0 = (t1 | 0x1000);
            ZoneFx_Zone2_StopVAlarms(p0, (t1 | 0x1000), t2, t0);
            return;
        }
    }
    if (t0 != 0) {
        return;
    }
    s32 t3 = *(u32 *)p0;
    s32 t4 = (*(u32 *)p0 & 0x1000);
    if (t4 == 0) {
        return;
    }
    *(u32 *)p0 = (t3 & ~0x1000);
    ZoneFx_Zone2_InitVAlarms(p0, (t3 & ~0x1000), t4);
}
