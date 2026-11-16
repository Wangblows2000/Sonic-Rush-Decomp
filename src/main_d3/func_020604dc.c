#pragma opt_common_subs off
#pragma opt_propagation off
#include <nitro/types.h>

typedef struct { char c[7]; } UnkStr7_020604dc;
extern char *data_0207ca5c[];
extern UnkStr7_020604dc data_0207cb6c;
extern char data_0207cb74[];

char *func_0202c474(char *dst, const char *src);
char *func_02060464(s32 a);
void *func_02033fbc(const char *path, s32 b);

#pragma thumb on
void *func_020604dc(s32 a, u32 idx) {
    char path[0x30] = {0};
    char **name;

    if (idx == 0 || idx >= 0x3a)
        return NULL;
    name = &data_0207ca5c[idx];
    if (data_0207ca5c[idx][0] != 0) {
        { UnkStr7_020604dc *d = (UnkStr7_020604dc *)path; const UnkStr7_020604dc *sp_ = &data_0207cb6c; *d = *sp_; }
        func_0202c474(path, *name);
        func_0202c474(path, func_02060464(a));
        func_0202c474(path, data_0207cb74);
        return func_02033fbc(path, -1);
    }
    return NULL;
}
#pragma thumb reset
