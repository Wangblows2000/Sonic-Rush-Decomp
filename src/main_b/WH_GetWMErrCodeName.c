#include <nitro/types.h>

extern void *data_020785ec[];
extern u8 data_02078b08;
void *WH_GetWMErrCodeName(s32 i) { if (i >= 0 && (u32)i < 23) return data_020785ec[i]; return &data_02078b08; }
