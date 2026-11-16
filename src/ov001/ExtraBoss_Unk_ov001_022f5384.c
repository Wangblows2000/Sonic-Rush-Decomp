#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StopAndFaceCenter2(void *p);
void ExtraBossObj_State_Unk_ov001_022f52e0(void);

void ExtraBoss_Unk_ov001_022f5384(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0x10, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
    *(s32 *)(p + 0x1cc) = 4;
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f52e0;
    ExtraBoss_StopAndFaceCenter2(p);
}
