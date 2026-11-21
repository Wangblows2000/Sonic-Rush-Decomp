#pragma opt_common_subs off
#pragma opt_propagation off
#include <nitro/types.h>

typedef struct { char c[7]; } UnkStr7_020604dc;
extern char *data_0207ca5c[];
extern UnkStr7_020604dc data_0207cb6c;
extern char data_0207cb74[];

char *strcat(char *dst, const char *src);
char *TalkDemo_GetLanguageSuffix(s32 a);
void *File_Load(const char *path, s32 b);

#pragma thumb on
void *TalkDemo_LoadScript(s32 a, u32 idx) {
    char path[0x30] = {0};
    char **name;

    if (idx == 0 || idx >= 0x3a)
        return NULL;
    name = &data_0207ca5c[idx];
    if (data_0207ca5c[idx][0] != 0) {
        { UnkStr7_020604dc *d = (UnkStr7_020604dc *)path; const UnkStr7_020604dc *sp_ = &data_0207cb6c; *d = *sp_; }
        strcat(path, *name);
        strcat(path, TalkDemo_GetLanguageSuffix(a));
        strcat(path, data_0207cb74);
        return File_Load(path, -1);
    }
    return NULL;
}
#pragma thumb reset
