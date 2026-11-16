/* paramclone of func_ov000_0235a270 (src/ov000_d/func_ov000_0235a270.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7e0];
    void *res[8];
} UnkStruct_022edcb0;

extern const char *data_ov001_0231d3a0[];
extern const char *data_ov001_0231d3c0[];
extern u8 gGameLanguage;

void *File_Load(const char *path, void *dst);

#pragma thumb on

void func_ov001_022edcb0(UnkStruct_022edcb0 *w) {
    s32 i;
    u8 cur;
    s32 lang;
    const char **tbl;

    i = 0;
    tbl = data_ov001_0231d3a0;
    cur = gGameLanguage;
    for (; i < 8; i++) {
        if (i == 1) {
            switch (cur) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                lang = cur;
                break;
            default:
                lang = 1;
                break;
            }
            w->res[i] = File_Load(data_ov001_0231d3c0[lang], NULL);
        } else {
            w->res[i] = File_Load(tbl[i], NULL);
        }
    }
}

#pragma thumb reset
