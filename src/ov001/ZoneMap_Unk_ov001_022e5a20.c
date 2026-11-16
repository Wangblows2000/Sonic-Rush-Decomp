#include <nitro/types.h>

void ZoneMap_Unk_ov001_022e5928(void *p);
void ZoneMap_Unk_ov001_022e57d4(void *p);

#pragma thumb on
void ZoneMap_Unk_ov001_022e5a20(s32 *p) {
    switch (*p) {
    case 0:
        ZoneMap_Unk_ov001_022e5928(p);
        break;
    case 1:
        ZoneMap_Unk_ov001_022e57d4(p);
        break;
    }
}
#pragma thumb reset
