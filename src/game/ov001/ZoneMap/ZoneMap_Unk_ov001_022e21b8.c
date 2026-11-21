#include <nitro/types.h>

void ZoneMap_SaveProgress(void *p);

#pragma thumb on
void ZoneMap_Unk_ov001_022e21b8(s32 *p) {
    s32 kind;
    s32 save;
    if (p[0] != 0) {
        kind = p[0x98 / 4];
        if (kind != 0x23) {
            save = p[2];
            if (kind == 0x13 || kind == 0x1b) {
                p[2] = kind;
                ZoneMap_SaveProgress(p);
            }
            p[2] = save;
        }
    }
}
#pragma thumb reset
