#include <nitro/types.h>

typedef struct { u8 pad[4]; u16 x4; u8 pad2[0x12]; u16 x18; u8 pad3[0x18]; u16 x32; } UnkStruct_02011970;
BOOL WmCheckParentParameter(UnkStruct_02011970 *p) {
    if (p->x4 > 0x70) return 0;
    if (p->x18 < 10 || p->x18 > 1000) return 0;
    if (p->x32 < 1 || p->x32 > 14) return 0;
    return 1;
}
