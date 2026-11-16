#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c0[];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StepXTowardCameraTarget(void *p);
u8 *ExtraBoss_GetWork(void);
s32 FX_DivS32(s32 a, s32 b);
void ExtraBoss_Unk_ov001_022fb5ec(void *p);
void ExtraBossObj_State_Unk_ov001_022f56f8(void);

void ExtraBossObj_State_Unk_ov001_022f57a8(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *d;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(r[0x10c] & 2)) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f56f8;
    Model_SetAnimation(r, 0, data_ov001_0231f068, data_ov001_0231d4c0[*(s32 *)(p + 0x1cc)] + 1, 0);
    a = ExtraBoss_GetWork();
    b = ExtraBoss_GetWork();
    c = ExtraBoss_GetWork();
    d = ExtraBoss_GetWork();
    *(s32 *)(r + 0x114) = FX_DivS32(*(s32 *)(a + 0x308) * 0x1333 + (*(s32 *)(b + 0x30c) - *(s32 *)(c + 0x308)) * 0x1800, *(s32 *)(d + 0x30c));
    ExtraBoss_Unk_ov001_022fb5ec(*(void **)(*(u8 **)(p + 0x1dc) + 0x10));
    *(s32 *)(p + 0x60) = 0;
    *(s32 *)(p + 0x5c) = *(s32 *)(p + 0x60);
    *(s32 *)(p + 0x54) = *(s32 *)(p + 0x5c);
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    *(u32 *)(p + 0xb8) |= 0x200;
}
