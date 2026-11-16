// asm: NitroSDK module-params block (BuildInfo) embedded in .text; data words only
#include <nitro/types.h>

extern u8 data_02086880[], data_02086898[], data_0207cfe0[];

asm void BuildInfo(void)
{
    dcd data_02086880
    dcd data_02086898
    dcd data_0207cfe0
    dcd data_0207cfe0
    dcd 0x022c5c80
    dcd 0x00000000
    dcd 0x02017532
    dcd 0xdec00621
    dcd 0x2106c0de
}
