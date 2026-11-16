/* paramclone of PlayerSelectMenu_LoadAssets (src/ov000_d/PlayerSelectMenu_LoadAssets.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7e0];
    void *res[8];
} UnkStruct_022edcb0;

extern const char *gRecordsFileList[];
extern const char *gRecordsLangFileList[];
extern u8 gGameLanguage;

void *File_Load(const char *path, void *dst);

#pragma thumb on

void Records_LoadAssets(UnkStruct_022edcb0 *w) {
    s32 i;
    u8 cur;
    s32 lang;
    const char **tbl;

    i = 0;
    tbl = gRecordsFileList;
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
            w->res[i] = File_Load(gRecordsLangFileList[lang], NULL);
        } else {
            w->res[i] = File_Load(tbl[i], NULL);
        }
    }
}

#pragma thumb reset
