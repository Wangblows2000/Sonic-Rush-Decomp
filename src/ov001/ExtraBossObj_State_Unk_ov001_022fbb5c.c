#include <nitro/types.h>

void ExtraBoss_Unk_ov001_022fb7f0(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ExtraBossObj_State_Unk_ov001_022fbaf0(void);

void ExtraBossObj_State_Unk_ov001_022fbb5c(u8 *p) {
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) > 0) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022fbaf0;
    *(s32 *)(p + 0x1cc) = 0x14;
    ExtraBoss_Unk_ov001_022fb7f0(p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), *(s32 *)(p + 0x20), 0x14, *(u16 *)(p + 0x40), *(u16 *)(p + 0x6c));
}
