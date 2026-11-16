#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022501a0;
extern UnkStruct_022501a0 data_022501a0;
void WirelessMgr_ChildReceiver(void *a, UnkStruct_022501a0 *src, s32 n) { if (n == 4) data_022501a0 = *src; }
