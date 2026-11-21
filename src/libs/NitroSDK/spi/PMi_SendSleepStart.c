#include <nitro/types.h>

extern vu32 data_0207f088;
extern vu32 data_0207f08c;

extern BOOL PMi_Lock(void);
extern void PMi_SendPxiData(u32 data);
extern void PMi_SetLCDPower(u32 a, u32 b, u32 c, u32 d);

u32 PMi_SendSleepStart(u32 a, u32 b)
{
    if (!PMi_Lock()) {
        return 1;
    }
    data_0207f088 = 0;
    PMi_SendPxiData(0x03006000);
    while (data_0207f088 == 0) {
    }
    data_0207f088 = 0;
    data_0207f08c = 0;
    PMi_SetLCDPower(0, 2, 0, 0);
    PMi_SendPxiData((a & 0xff) | 0x02006100);
    PMi_SendPxiData((b & 0xffff) | 0x01010000);
    return 0;
}
