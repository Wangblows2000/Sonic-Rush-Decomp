#include <nitro/types.h>

void func_ov001_022e2480(void *p);

#pragma thumb on
void func_ov001_022e21b8(s32 *p) {
    s32 kind;
    s32 save;
    if (p[0] != 0) {
        kind = p[0x98 / 4];
        if (kind != 0x23) {
            save = p[2];
            if (kind == 0x13 || kind == 0x1b) {
                p[2] = kind;
                func_ov001_022e2480(p);
            }
            p[2] = save;
        }
    }
}
#pragma thumb reset
