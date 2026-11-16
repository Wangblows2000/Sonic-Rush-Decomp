#include <nitro/types.h>

void ExtraBoss_Unk_ov001_022fa558(void *p);
void ExtraBoss_Unk_ov001_022fd268(void *p, s32 a);
void ExtraBossObj_State_Unk_ov001_022fa664(void);

void ExtraBossObj_State_Unk_ov001_022fa6e8(u8 *p) {
    ExtraBoss_Unk_ov001_022fa558(p);
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) > 0) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022fa664;
    ExtraBoss_Unk_ov001_022fd268(p, 2);
    (*(u8 **)(p + 0x84))[0x10c] &= ~4;
    (*(u8 **)(p + 0x84))[0x10d] &= ~4;
    (*(u8 **)(p + 0x84))[0x10e] &= ~4;
    *(s32 *)(p + 0x1cc) = 0xa;
}
