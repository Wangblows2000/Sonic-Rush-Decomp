#include <nitro/types.h>

extern void *data_022c4600[];
extern u8 gGameLanguage;
extern const char **data_0207a80c[];
void *File_Load(const char *path, void *dst);

#pragma thumb on
void func_0204f5a8(s32 idx) {
    void **slot = &data_022c4600[idx];
    if (*slot == NULL) {
        u32 lang;
        switch (gGameLanguage) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            lang = gGameLanguage;
            break;
        default:
            lang = 1;
            break;
        }
        *slot = File_Load(data_0207a80c[lang][idx], NULL);
    }
}
#pragma thumb reset
