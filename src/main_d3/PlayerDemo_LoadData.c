#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    char s[7];
} UnkPre_020570f4;

extern s32 data_02076214[];
extern char *data_0207c368[];
extern UnkPre_020570f4 data_0207c480;
extern char data_0207c488[];

void func_0202c474(char *dst, const char *src);
void *File_Load(char *path, s32 b);

#pragma thumb on
void *PlayerDemo_LoadData(s32 a) {
    char buf[0x30] = {0};
    char *s;

    if (a == 0 || a >= 0x2c)
        return NULL;
    s = data_0207c368[data_02076214[a]];
    if (s[0] == 0)
        return NULL;
    { UnkPre_020570f4 *d = (UnkPre_020570f4 *)buf; const UnkPre_020570f4 *sp_ = &data_0207c480; *d = *sp_; }
    func_0202c474(buf, s);
    func_0202c474(buf, data_0207c488);
    return File_Load(buf, -1);
}
#pragma thumb reset
