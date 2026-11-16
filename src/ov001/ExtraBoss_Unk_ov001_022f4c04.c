#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBossObj_State_Unk_ov001_022f4a50(void);

void ExtraBoss_Unk_ov001_022f4c04(u8 *p) {
    s32 *a = *(s32 **)(p + 0x1dc);
    u8 *q = *(u8 **)(p + 0x84);
    a[2] = a[0];
    a[0] = 2;
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f4a50;
    q[0x10c] |= 0x40;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 1, 0);
    q[0x10c] &= ~4;
    *(u16 *)(q + 0x12c) = 0x400;
    *(s32 *)(q + 0x114) = 0x1000;
    *(s32 *)(p + 0x50) = 0x6000;
    *(s32 *)(p + 0x60) = 0;
    *(s32 *)(p + 0x5c) = *(s32 *)(p + 0x60);
    *(s32 *)(p + 0x54) = *(s32 *)(p + 0x5c);
    *(u32 *)(p + 0xb8) |= 0x200;
    *(u32 *)(p + 0xf0) |= 0x200;
    *(u16 *)(p + 0x1c8) = 0;
    *(u16 *)(p + 0x1cc) = 0;
}
