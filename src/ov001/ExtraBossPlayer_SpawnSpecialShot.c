#include <nitro/types.h>

u8 *ExtraBossPlayer_CreateFxObj(s32 a, void *b, s32 c, s32 d, s32 e);
void ExtraBoss_InitCollider(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ExtraBossObj_State_Unk_ov001_022fd1ec(void);

void ExtraBossPlayer_SpawnSpecialShot(u8 *p, s32 b, s32 c, s32 d) {
    u8 *t = ExtraBossPlayer_CreateFxObj(0, p, b, c, d);
    if (t == NULL) {
        return;
    }
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fd1ec;
    *(u32 *)(t + 0x14) &= ~1;
    *(void **)(t + 0x80) = *(void **)(p + 0x80);
    (*(u8 **)(t + 0x84))[0x10f] |= 4;
    *(s32 *)(t + 0x50) = 0x5000;
    ExtraBoss_InitCollider(t + 0xe8, t, 6, 1, 0x10000, 0x10000, 0x10000, 0, 0, 0, 0);
    *(u8 **)(t + 0xa0) = t + 0xe8;
}
