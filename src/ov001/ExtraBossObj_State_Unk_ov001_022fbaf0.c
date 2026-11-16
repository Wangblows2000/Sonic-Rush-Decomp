#include <nitro/types.h>

void ExtraBossObj_State_Unk_ov001_022fbbc8(void);
void ExtraBossObj_State_Unk_ov001_022fbb5c(void);

void ExtraBossObj_State_Unk_ov001_022fbaf0(u8 *p) {
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) > 0) {
        return;
    }
    *(s32 *)(p + 0x1d0) -= 1;
    if (*(s32 *)(p + 0x1d0) <= 0) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022fbbc8;
        *(s32 *)(p + 0x1cc) = 0x78;
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022fbb5c;
    *(s32 *)(p + 0x54) = 0;
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    *(s32 *)(p + 0x1cc) = 0x1e;
}
