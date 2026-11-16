#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1e08];
    void *res[16];
} UnkStruct_022ce9b0;

extern u8 gGameLanguage;
extern char **data_ov001_0231a9a4[];

void *File_Load(char *name, s32 a);

#pragma thumb on
void func_ov001_022ce9b0(UnkStruct_022ce9b0 *p) {
    u32 lang;
    char **names;
    s32 i;

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
    names = data_ov001_0231a9a4[lang];
    for (i = 0; i < 16; i++) {
        p->res[i] = File_Load(names[i], 0);
    }
}
#pragma thumb reset
