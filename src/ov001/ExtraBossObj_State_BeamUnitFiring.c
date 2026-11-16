#include <nitro/types.h>

void ExtraBossObj_State_BeamUnitActive(void);
void ExtraBossObj_State_BeamUnitCharge(void);

void ExtraBossObj_State_BeamUnitFiring(u8 *p) {
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) > 0) {
        return;
    }
    *(s32 *)(p + 0x1d0) -= 1;
    if (*(s32 *)(p + 0x1d0) <= 0) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_BeamUnitActive;
        *(s32 *)(p + 0x1cc) = 0x78;
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_BeamUnitCharge;
    *(s32 *)(p + 0x54) = 0;
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    *(s32 *)(p + 0x1cc) = 0x1e;
}
