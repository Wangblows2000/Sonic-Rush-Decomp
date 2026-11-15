#include <nitro/types.h>
#include <main_sdk2/wh.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
s32 func_02015e28(u32 type, u16 bitmap);
s32 func_02015c78(void);
s32 func_02015924(void);

void func_02015790(void)
{
    u16 m[5];
    u16 i;
    s32 r;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, m, sizeof(m));
    }
    for (i = 1; i <= 15; i++) {
        switch (data_02080f1c->unk14e8[i - 1]) {
        case 2:
            m[0] |= 1 << i;
            break;
        case 5:
            m[1] |= 1 << i;
            break;
        case 4:
            m[2] |= 1 << i;
            break;
        case 8:
            m[3] |= 1 << i;
            break;
        case 11:
            m[4] |= 1 << i;
            break;
        }
    }
    if (m[3] != 0) {
        r = func_02015e28(5, m[3]);
    } else if (m[0] != 0) {
        r = func_02015e28(1, m[0]);
    } else if (m[4] != 0) {
        r = func_02015e28(6, m[4]);
    } else if (m[2] != 0) {
        r = func_02015e28(2, m[2]);
    } else if (m[1] != 0) {
        r = func_02015c78();
    } else {
        r = func_02015924();
    }
    if (r == 0x15) {
        func_02015e28(0, 0xffff);
    }
}
