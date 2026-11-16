#include <nitro/types.h>

void ExtraBoss_Unk_ov001_022fa558(void *p);
void ExtraBoss_Unk_ov001_022fd268(void *p, s32 a);
u32 *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_02303af8(void);

void ExtraBossObj_State_Unk_ov001_022fa664(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    ExtraBoss_Unk_ov001_022fa558(p);
    *(s32 *)(p + 0x1cc) -= 1;
    if (q[0x10c] & 2) {
        ExtraBoss_Unk_ov001_022fd268(p, 0);
        *ExtraBoss_GetWork() &= ~0x40;
        ExtraBoss_Unk_ov001_02303af8();
        *(u32 *)(p + 0x14) |= 8;
        return;
    }
    if (*(s32 *)(p + 0x1cc) == 0) {
        *(u32 *)(p + 0xf0) &= ~0x200;
    }
}
