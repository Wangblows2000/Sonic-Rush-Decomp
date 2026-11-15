#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_0201299c(void *ds);

typedef struct {
    u16 apiid;
    u16 errcode;
    u32 unk4;
} UnkWmCb_02012d20;

s32 func_02012d20(void (*cb)(void *))
{
    u8 *sys = func_02010b70();
    UnkWmCb_02012d20 arg;
    s32 r = func_02010a50(2, 9, 10);
    if (r != 0) {
        return r;
    }
    if (*(u32 *)(sys + 0x15c) == 0) {
        return 3;
    }
    r = func_0201299c(sys + 0x160);
    *(u32 *)(sys + 0x15c) = 0;
    arg.apiid = 0x16;
    arg.errcode = r;
    cb(&arg);
    return 2;
}
