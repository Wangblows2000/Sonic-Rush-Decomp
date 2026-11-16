#include <nitro/types.h>

extern void *data_ov000_0236aab8;
extern u8 data_ov000_02368804[];
extern u32 data_02250438;
extern u32 data_02250434;

void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
s32 func_ov000_0233ac84(void *res);
u32 func_ov000_0233ac78(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *res);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020356bc(void *res, s32 a, u32 b);
void func_02035754(void);
void func_ov000_0233a4c8(void *f);

void func_ov000_0233ace4(void) {
    u8 f[0x5C];
    void *res;
    u32 sa;
    u32 sb;

    func_02021310(f, data_ov000_02368804, data_ov000_0236aab8);
    res = func_02021264(f, 3);
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, (void *)0x6010000);
        break;
    case 0x20:
        func_02008720(res, (void *)0x6010000);
        break;
    case 0x30:
        func_020087f8(res, (void *)0x6010000);
        break;
    default:
        func_02033a68(func_02033a54(res), (void *)0x6010000, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange((void *)0x6010000, func_ov000_0233ac78(res));
    res = func_02021264(f, 6);
    sa = data_02250438;
    sb = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_020356bc(res, 0, 0x5000000);
    func_02035754();
    data_02250438 = sa;
    data_02250434 = sb;
    func_ov000_0233a4c8(f);
    func_020212e0(f);
}
