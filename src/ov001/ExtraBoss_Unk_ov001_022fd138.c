#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);
u8 *ExtraBossPlayer_CreateFxObj(s32 a, void *b, s32 c, s32 d, s32 e);
void ExtraBossObj_State_Unk_ov001_022fd000(void);

void ExtraBoss_Unk_ov001_022fd138(u8 *p) {
    s32 k = 0x17;
    u8 *t;
    if (*(s32 *)(ExtraBoss_GetWork() + 0x3c) == 1) {
        k = 0x19;
    }
    t = ExtraBossPlayer_CreateFxObj(k, p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), *(s32 *)(p + 0x20));
    if (t == NULL) {
        return;
    }
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fd000;
    *(u32 *)(t + 0x14) |= 0x20;
    (*(u8 **)(t + 0x84))[0x10c] |= 4;
    (*(u8 **)(t + 0x84))[0x10d] |= 4;
}
