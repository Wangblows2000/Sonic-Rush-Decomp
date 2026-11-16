#include <nitro/types.h>

u8 *ExtraBoss_GetPlayer(void);
void ExtraBossPlayer_Action_UnlockControl(void *p);
void func_ov001_022fd138(void *p);
void func_ov001_022ee594(void *p);

void func_ov001_02302a3c(void) {
    u8 *p = ExtraBoss_GetPlayer();
    u32 *a = *(u32 **)(p + 0x1dc);
    ExtraBossPlayer_Action_UnlockControl(p);
    *(u32 *)(p + 0x14) &= 0xffe4efff;
    *a |= 1;
    *(s32 *)(p + 0x24) = 0x40000;
    *(s32 *)(p + 0x18) = *(s32 *)(p + 0x24);
    *(s32 *)(p + 0x28) = 0xc0000;
    *(s32 *)(p + 0x1c) = *(s32 *)(p + 0x28);
    *(s32 *)(p + 0x2c) = 0;
    *(s32 *)(p + 0x20) = *(s32 *)(p + 0x2c);
    *(u8 **)(p + 0x9c) = p + 0xb0;
    func_ov001_022fd138(p);
    func_ov001_022ee594(p);
}
