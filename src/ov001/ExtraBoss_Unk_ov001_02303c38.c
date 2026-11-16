#include <nitro/types.h>

extern u8 *data_ov001_0231f548;
void ExtraBoss_Unk_ov001_02303594(void);

void ExtraBoss_Unk_ov001_02303c38(void) {
    *(void (**)(void))(data_ov001_0231f548 + 0x90) = ExtraBoss_Unk_ov001_02303594;
}
