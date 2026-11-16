#include <nitro/types.h>

extern u8 *data_ov001_0231f548;
extern u8 data_ov001_02316cac[];
extern u8 data_ov001_02316b9c[];
void ExtraBoss_Unk_ov001_02303c54(void);
u8 *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_02303934(void);

void ExtraBoss_Unk_ov001_02304460(void) {
    ExtraBoss_Unk_ov001_02303c54();
    *(s32 *)(data_ov001_0231f548 + 0x137c) = 0;
    *(s32 *)(data_ov001_0231f548 + 0x1380) = *(s32 *)(data_ov001_0231f548 + 0x137c);
    if (*(s32 *)(ExtraBoss_GetWork() + 0x40) == 4) {
        *(void **)(data_ov001_0231f548 + 0x1384) = data_ov001_02316b9c;
    } else {
        *(void **)(data_ov001_0231f548 + 0x1384) = data_ov001_02316cac;
    }
    *(void (**)(void))(data_ov001_0231f548 + 0x90) = ExtraBoss_Unk_ov001_02303934;
    *(u32 *)(data_ov001_0231f548 + 0x94) |= 3;
}
