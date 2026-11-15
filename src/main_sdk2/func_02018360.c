#include <nitro/types.h>
#include <main_sdk2/wh.h>

u8 func_02018360(void)
{
    UnkParentParam_wh *p = &data_02080f18->parentParam;
    return (u8)((p->entryFlag ? 1 : 0) | (p->multiBootFlag ? 2 : 0) | (p->KS_Flag ? 4 : 0) | (p->CS_Flag ? 8 : 0));
}
