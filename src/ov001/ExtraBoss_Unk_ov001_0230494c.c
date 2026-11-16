#include <nitro/types.h>

extern u8 *data_ov001_0231f54c;
void ExtraBoss_Unk_ov001_02304834(void);
u8 *ExtraBoss_GetPlayer(void);
void ExtraBoss_Unk_ov001_022fc4d4(s32 a, s32 b, s32 c);
u8 *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_022ee964(void);
void ExtraBoss_Unk_ov001_02304948(void);

void ExtraBoss_Unk_ov001_0230494c(void) {
    u8 *g;
    ExtraBoss_Unk_ov001_02304834();
    *(s32 *)(data_ov001_0231f54c + 0xb44) += 1;
    g = data_ov001_0231f54c;
    if (*(s32 *)(g + 0xb44) >= *(s32 *)(g + 0xb48)) {
        *(s32 *)(g + 0xb44) = 0;
        ExtraBoss_Unk_ov001_022fc4d4(0, *(s32 *)(ExtraBoss_GetPlayer() + 0x1c), 0);
    }
    if (*(s32 *)(data_ov001_0231f54c + 0x588) < 0xe10000) {
        return;
    }
    g = ExtraBoss_GetWork();
    *(void (**)(void))(data_ov001_0231f54c + 0x598) = ExtraBoss_Unk_ov001_02304948;
    *(s32 *)(g + 0x50) = 0;
    *(s32 *)(g + 0x54) = 0;
    *(s32 *)(g + 0x58) = 0x180000;
    *(s32 *)(g + 0x5c) = 0x180000;
    ExtraBoss_Unk_ov001_022ee964();
}
