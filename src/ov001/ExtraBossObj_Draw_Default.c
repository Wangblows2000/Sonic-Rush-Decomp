#include <nitro/types.h>

void func_ov001_0230063c(void *p);
void func_ov001_02300978(void *p);

void ExtraBossObj_Draw_Default(u8 *p) {
    if (*(void **)(p + 0x94) != NULL) {
        func_ov001_0230063c(p);
    } else if (*(void **)(p + 0x84) != NULL) {
        func_ov001_0230063c(p);
    } else if (*(void **)(p + 0x90) != NULL) {
        func_ov001_02300978(p);
    }
}
