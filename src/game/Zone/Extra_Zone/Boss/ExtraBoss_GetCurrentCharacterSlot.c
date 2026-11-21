/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();
extern u8 data_ov001_0231f080[];

s32 ExtraBoss_GetCurrentCharacterSlot(void) {
    s32 t0 = ExtraBoss_GetWork();
    return (*(u32 *)((u8 *)t0 + 0x4) * 0x44 + *(u32 *)data_ov001_0231f080);
}
