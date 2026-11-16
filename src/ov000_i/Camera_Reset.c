#include <nitro/types.h>

extern u8 gCamera[];
extern u8 data_ov000_0236aabc[];
extern u16 data_ov000_0236aab0;
extern u32 data_ov000_0236aab8;
extern u32 data_ov000_0236aab4;
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void ZoneBg_Reset(void);
void ZoneFx_Reset(void);

void Camera_Reset(void) {
    vu16 z0;
    vu16 z1;

    z0 = 0;
    MIi_CpuClear16(z0, gCamera, 0x140);
    z1 = 0;
    MIi_CpuClear16(z1, data_ov000_0236aabc, 0x1c);
    data_ov000_0236aab0 = 0;
    data_ov000_0236aab8 = 0;
    data_ov000_0236aab4 = 0;
    ZoneBg_Reset();
    ZoneFx_Reset();
}
