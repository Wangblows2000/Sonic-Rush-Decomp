#include <nitro/types.h>

extern void *data_022c4600[];
extern u8 data_02072cb8;
extern const char **data_0207a80c[];
void *func_02033fbc(const char *path, void *dst);

#pragma thumb on
void func_0204f5a8(s32 idx) {
    void **slot = &data_022c4600[idx];
    if (*slot == NULL) {
        u32 lang;
        switch (data_02072cb8) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            lang = data_02072cb8;
            break;
        default:
            lang = 1;
            break;
        }
        *slot = func_02033fbc(data_0207a80c[lang][idx], NULL);
    }
}
#pragma thumb reset
