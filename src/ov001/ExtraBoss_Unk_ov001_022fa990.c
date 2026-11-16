#include <nitro/types.h>

u8 *ExtraBossPlayer_CreateFxObj(s32 a, s32 b, s32 c, s32 d, s32 e);
void *ExtraBoss_GetPlayer(void);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void ExtraBossObj_State_Unk_ov001_022fd1c8(void);

void ExtraBoss_Unk_ov001_022fa990(s32 a, s32 b, s32 c, s32 d, s16 big) {
    u8 *t;
    if (big != 0) {
        t = ExtraBossPlayer_CreateFxObj(0x11, a, b, c, d);
    } else {
        t = ExtraBossPlayer_CreateFxObj(0x10, a, b, c, d);
    }
    if (t == NULL) {
        return;
    }
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fd1c8;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u16 *)(t + 0x98) = 0xffff;
    *(s32 *)(t + 0x1cc) = 0x3c;
    Sound_PlaySeqArc(0, 0, 0xb2);
}
