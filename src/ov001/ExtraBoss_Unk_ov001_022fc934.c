#include <nitro/types.h>

u8 *ExtraBossPlayer_CreateFxObj(s32 a, s32 b, s32 c, s32 d, s32 e);
void *ExtraBoss_GetPlayer(void);
void ExtraBoss_InitCollider(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);

void ExtraBoss_Unk_ov001_022fc934(s32 a, s32 b, s32 c, s32 d, BOOL flag) {
    u8 *t = ExtraBossPlayer_CreateFxObj(0xd, a, b, c, d);
    if (t == NULL) {
        return;
    }
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u16 *)(t + 0x98) = 0xffff;
    ExtraBoss_InitCollider(t + 0xe8, t, 1, 1, 0x1e000, 0x1e000, 0x10000, 0, 0, 0, 0);
    if (flag) {
        *(u32 *)(t + 0xf0) |= 0x10;
    }
    *(u8 **)(t + 0xa0) = t + 0xe8;
}
