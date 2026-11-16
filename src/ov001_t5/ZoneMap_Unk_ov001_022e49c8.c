#include <nitro/types.h>

s32 ZoneMap_IsBlaze(void *a, void *b);
void ZoneMap_Unk_ov001_022e4748(void *a, void *b);
void ZoneMap_Unk_ov001_022e4198(void *a, void *b);
void ZoneMap_Unk_ov001_022e4494(void *a, void *b);
void ZoneMap_Unk_ov001_022e4000(void *a, void *b);
void ZoneMap_Unk_ov001_022e432c(void *a, void *b);

#pragma thumb on
void ZoneMap_Unk_ov001_022e49c8(void *a, void *b) {
    switch (ZoneMap_IsBlaze(a, b)) {
    case 0:
        ZoneMap_Unk_ov001_022e4748(a, b);
        ZoneMap_Unk_ov001_022e4198(a, b);
        break;
    case 1:
        ZoneMap_Unk_ov001_022e4494(a, b);
        ZoneMap_Unk_ov001_022e4000(a, b);
        break;
    }
    ZoneMap_Unk_ov001_022e432c(a, b);
}
#pragma thumb reset
