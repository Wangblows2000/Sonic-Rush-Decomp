#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
} UnkRes_02341d00;

extern u32 data_02250434;
extern u32 data_02250438;
extern u32 data_0225043c;
extern u32 data_02250440;

s32 func_ov000_023418b8(void *p);
u32 func_ov000_023418ac(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020356bc(void *p, s32 a, s32 b);
void func_02035754(void);
void func_02035d04(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_020363f0(void);

void func_ov000_02341d00(UnkRes_02341d00 *res, s32 mode, u32 flags) {
    u8 *p;
    void *src;
    u32 a;
    u32 b;
    s32 f;

    if (mode == 1) {
        return;
    }
    if (flags & 2) {
        p = (u8 *)res + res->unk8;
        switch (func_ov000_023418b8(p)) {
        case 0x10:
            func_02008648(p, (void *)0x6008000);
            break;
        case 0x20:
            func_02008720(p, (void *)0x6008000);
            break;
        case 0x30:
            func_020087f8(p, (void *)0x6008000);
            break;
        default:
            src = func_02033a54(p);
            func_02033a68(src, (void *)0x6008000, func_ov000_023418ac(p));
            break;
        }
        DC_StoreRange((void *)0x6008000, func_ov000_023418ac(p));
    }
    if (flags & 1) {
        p = (u8 *)res + res->unk10;
        a = data_02250438;
        b = data_02250434;
        data_02250438 = 0;
        data_02250434 = 0;
        func_020356bc(p, 0, 0x5000000);
        func_02035754();
        data_02250438 = a;
        data_02250434 = b;
    }
    if (flags & 4) {
        p = (u8 *)res + res->unkC;
        f = (res->unk14 & 0x1000) ? 1 : 0;
        a = data_02250440;
        b = data_0225043c;
        data_02250440 = 0;
        data_0225043c = 0;
        func_02035d04(p, 0, 0, 0x40, f, 3, 0, 8, 0, 0, 0x40, 0x40);
        func_020363f0();
        data_02250440 = a;
        data_0225043c = b;
    }
}
