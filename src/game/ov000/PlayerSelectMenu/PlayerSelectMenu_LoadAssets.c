#include <nitro/types.h>

typedef struct {
    u8 pad0[0xCF4];
    void *res[15];
} UnkStruct_0235a270;

extern const char *data_ov000_02369094[];
extern const char *data_ov000_02369074[];
extern u8 gGameLanguage;

void *File_Load(const char *path, void *dst);

#pragma thumb on

void PlayerSelectMenu_LoadAssets(UnkStruct_0235a270 *w) {
    s32 i;
    u8 cur;
    s32 lang;
    const char **tbl;

    i = 0;
    tbl = data_ov000_02369094;
    cur = gGameLanguage;
    for (; i < 15; i++) {
        if (i == 11) {
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
            w->res[i] = File_Load(data_ov000_02369074[lang], NULL);
        } else {
            w->res[i] = File_Load(tbl[i], NULL);
        }
    }
}

#pragma thumb reset
