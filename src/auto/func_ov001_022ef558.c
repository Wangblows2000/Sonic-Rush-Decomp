/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022f71f4();
s32 BrightnessLerp_Create();
s32 func_ov001_022ef11c();
s32 ExtraBoss_GetWork();
extern u8 data_ov001_023167f8[];
extern u8 data_ov001_0231f070[];
extern u8 gExtraBossArchive[];
extern u8 data_ov001_0231d4ac[];
extern u8 data_ov001_02316804[];
s32 func_ov001_022ffb9c();
extern u8 data_ov001_023167ec[];

void func_ov001_022ef558(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    func_ov001_022f71f4();
    *(u16 *)((u8 *)p0 + 0x1c8) = (*(s16 *)((u8 *)p0 + 0x1c8) + 0x1);
    *(u16 *)((u8 *)p0 + 0x1cc) = (*(s16 *)((u8 *)p0 + 0x1cc) + 0x43);
    s32 t1 = *(s16 *)((u8 *)p0 + 0x1cc);
    if (t1 > 0x1000) {
        *(u16 *)((u8 *)p0 + 0x1cc) = 0x1000;
    }
    if (*(s16 *)((u8 *)p0 + 0x1c8) == 0x77) {
        BrightnessLerp_Create(0x10, 0, 0x10, 3, 0);
    }
    if ((*(u8 *)((u8 *)t0 + 0x10c) & 2) != 0) {
        func_ov001_022ef11c(p0);
        s32 t2 = ExtraBoss_GetWork();
        *(u32 *)((u8 *)t2 + 0x318) = 0;
        func_ov001_022ffb9c(*(u32 *)data_ov001_0231f070, *(u32 *)gExtraBossArchive, data_ov001_0231d4ac, data_ov001_02316804, data_ov001_023167f8, 6);
        return;
    }
    s32 t3 = (*(s16 *)((u8 *)p0 + 0x1c8) & 7);
    if (t3 == 1) {
        func_ov001_022ffb9c(*(u32 *)data_ov001_0231f070, *(u32 *)gExtraBossArchive, data_ov001_0231d4ac, data_ov001_023167ec, data_ov001_023167f8, 6);
        return;
    }
    if (t3 != 5) {
        return;
    }
    func_ov001_022ffb9c(*(u32 *)data_ov001_0231f070, *(u32 *)gExtraBossArchive, data_ov001_0231d4ac, data_ov001_02316804, data_ov001_023167f8, 6);
}
