#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c4[];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
u8 *ExtraBoss_Unk_ov001_022fb678(void *p);
void ExtraBoss_StopAndFaceCenter2(void *p);
void ExtraBossObj_State_Unk_ov001_022f57a8(void);

void ExtraBoss_Unk_ov001_022f589c(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *q = *(u8 **)(p + 0x1dc);
    u8 *t;
    u8 *s;
    Model_SetAnimation(r, 0, data_ov001_0231f068, data_ov001_0231d4c4[1], 0);
    r[0x10c] &= ~4;
    *(s32 *)(r + 0x114) = 0x1000;
    *(s32 *)(p + 0x1cc) = 2;
    t = ExtraBoss_Unk_ov001_022fb678(p);
    *(u8 **)(q + 0x10) = t;
    s = *(u8 **)(t + 0x84);
    *(u16 *)(s + 8) = 0x1d;
    s[0xa] = 4;
    s[0xb] = 2;
    s[0xc] = 0xb;
    *(u16 *)(t + 0x3c) = 0x4000;
    *(u16 *)(t + 0x3e) = 0x8000;
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f57a8;
    ExtraBoss_StopAndFaceCenter2(p);
}
