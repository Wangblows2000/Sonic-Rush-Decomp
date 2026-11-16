#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBossObj_State_Unk_ov001_022f62f0(void);

void ExtraBoss_Unk_ov001_022f63dc(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f62f0;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0, 0);
    q[0x10c] |= 4;
    *(s32 *)(q + 0x114) = 0x1000;
    *(u16 *)(p + 0x1c8) = 0;
    *(u32 *)(p + 0xb8) |= 0x200;
}
