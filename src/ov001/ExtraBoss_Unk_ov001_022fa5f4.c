#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);
u8 *ExtraBossPlayer_CreateFxObj(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 ExtraBoss_GetPlayer(void);
void ExtraBossObj_State_Unk_ov001_022fa5c8(void);

void ExtraBoss_Unk_ov001_022fa5f4(s32 a, s32 b, s32 c) {
    s32 v = *(s32 *)(ExtraBoss_GetWork() + 0x31c);
    u8 *t = ExtraBossPlayer_CreateFxObj(0x15, a, v, b, c);
    if (t == NULL) {
        return;
    }
    (*(u8 **)(t + 0x84))[0x10f] |= 4;
    *(s32 *)(t + 0x80) = ExtraBoss_GetPlayer();
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fa5c8;
}
