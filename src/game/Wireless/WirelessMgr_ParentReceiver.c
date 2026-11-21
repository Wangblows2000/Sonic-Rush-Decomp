#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
} UnkStruct_022501a0;

extern UnkStruct_022501a0 data_022501a0;

void WirelessMgr_ParentReceiver(u8 bit, u32 unused, s32 kind)
{
    if (kind == 4) {
        ((u16 *)&data_022501a0)[1] |= 1 << bit;
    }
}
