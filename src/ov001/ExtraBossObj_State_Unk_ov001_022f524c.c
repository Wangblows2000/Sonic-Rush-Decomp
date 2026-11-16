#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StepXTowardCameraTarget(void *p);
void ExtraBossObj_State_Unk_ov001_022f51a8(void);
void ExtraBossObj_State_Unk_ov001_022f5108(void);

void ExtraBossObj_State_Unk_ov001_022f524c(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    *(s32 *)(p + 0x1d0) -= 1;
    if (*(s32 *)(p + 0x1d0) > 0) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f51a8;
        *(s32 *)(p + 0x1c8) = 0xa;
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022f5108;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0x12, 0);
}
