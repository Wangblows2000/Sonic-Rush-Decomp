#include <nitro/types.h>

void ZoneMap_Unk_ov001_022e3678(void *p);
void ZoneMap_Unk_ov001_022e3608(void *p);

#pragma thumb on
void ZoneMap_Unk_ov001_022e3544(u8 *p) {
    u8 *q = *(u8 **)(p + 0xa4) + 0x1b08;
    ZoneMap_Unk_ov001_022e3678(q);
    ZoneMap_Unk_ov001_022e3608(q);
}
#pragma thumb reset
