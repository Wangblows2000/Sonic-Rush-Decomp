#include <nitro/types.h>

typedef struct {
    u8 pad[0x550];
    void *files[1];
} UnkObj_0204f4d4;

extern void *data_022c4600[];
extern u8 gGameLanguage;
extern const char **data_0207a80c[];
void *File_Load(const char *path, void *dst);

#pragma thumb on
void MsgBox_Unk_0204f4d4(UnkObj_0204f4d4 *o, s32 idx) {
    void *p = data_022c4600[idx];
    if (p != NULL) {
        o->files[idx] = p;
    } else {
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
        o->files[idx] = File_Load(data_0207a80c[lang][idx], NULL);
    }
}
#pragma thumb reset
