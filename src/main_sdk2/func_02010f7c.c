#include <nitro/types.h>

typedef struct {
    u32 hour;
    u32 minute;
    u32 second;
} UnkRtcTime;

extern u32 data_0207799c;

void func_0200f160(void);
s32 func_0200f0a0(UnkRtcTime *t);

u16 func_02010f7c(void)
{
    if (data_0207799c == 0x10000) {
        UnkRtcTime t;
        func_0200f160();
        if (func_0200f0a0(&t) == 0) {
            data_0207799c = (u16)(t.second + (t.minute << 8));
        }
    }
    data_0207799c = (u16)(data_0207799c + 1);
    return data_0207799c;
}
