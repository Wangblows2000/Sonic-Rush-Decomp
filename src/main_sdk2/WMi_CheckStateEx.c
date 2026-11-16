#include <nitro/types.h>
#include <main_sdk2/stdarg.h>

typedef struct {
    u32 unk0;
    u16 *state;
} UnkWmSys_02010a50;

extern UnkWmSys_02010a50 *data_0207faec;

s32 func_02010b54(void);
void DC_InvalidateRange(void *p, u32 size);

s32 WMi_CheckStateEx(s32 num, ...)
{
    va_list ap;
    u16 state;
    s32 r = func_02010b54();
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(data_0207faec->state, 2);
    state = *data_0207faec->state;
    va_start(ap, num);
    r = 3;
    while (num != 0) {
        if (va_arg(ap, u32) == state) {
            r = 0;
        }
        num--;
    }
    va_end(ap);
    return r;
}
