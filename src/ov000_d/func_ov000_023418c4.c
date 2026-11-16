#include <nitro/types.h>

typedef struct UnkStruct_023418c4 {
    u8 pad0[8];
    s32 chr;
    s32 scr;
    s32 pal;
    u32 flags;
} UnkStruct_023418c4;

extern u32 data_02250438;
extern u32 data_02250434;
extern u32 data_02250440;
extern u32 data_0225043c;

u32 func_ov000_023418b8(void *res);
u32 func_ov000_023418ac(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *res);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020356bc(void *res, s32 a, u32 b);
void func_02035754(void);
void func_02035d04(void *res, s32 a, s32 b, s32 k, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
void func_020363f0(void);

static inline void UnkDecomp_023418c4(void *res, void *dst) {
    switch (func_ov000_023418b8(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_023418ac(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_023418ac(res));
}

static inline void UnkPal_023418c4(void *res, s32 a, u32 b) {
    u32 sa = data_02250438;
    u32 sb = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_020356bc(res, a, b);
    func_02035754();
    data_02250438 = sa;
    data_02250434 = sb;
}

static inline void UnkScr_023418c4(UnkStruct_023418c4 *r, s32 x) {
    void *res = (u8 *)r + r->scr;
    s32 flag = (r->flags & 0x1000) ? 1 : 0;
    u32 sa = data_02250440;
    u32 sb = data_0225043c;
    data_02250440 = 0;
    data_0225043c = 0;
    func_02035d04(res, 0, 0, 0x40, flag, x, 0, 8, 0, 0, 0x40, 0x40);
    func_020363f0();
    data_02250440 = sa;
    data_0225043c = sb;
}

void func_ov000_023418c4(UnkStruct_023418c4 *r, s32 screen, u32 mask) {
    u32 type = r->flags & 0xf;

    if (screen == 0 || screen == 2) {
        if (mask & 2) {
            UnkDecomp_023418c4((u8 *)r + r->chr, (void *)0x6008000);
        }
        if (mask & 1) {
            switch (type) {
            case 1:
                UnkPal_023418c4((u8 *)r + r->pal, 0, 0x5000000);
                break;
            case 2:
                UnkPal_023418c4((u8 *)r + r->pal, 1, 0x6200);
                break;
            }
        }
        if (mask & 4) {
            UnkScr_023418c4(r, 3);
        }
    }
    if ((u32)(screen - 1) > 1) {
        return;
    }
    if (mask & 2) {
        UnkDecomp_023418c4((u8 *)r + r->chr, (void *)0x6208000);
    }
    if (mask & 1) {
        switch (type) {
        case 1:
            UnkPal_023418c4((u8 *)r + r->pal, 0, 0x5000400);
            break;
        case 2:
            UnkPal_023418c4((u8 *)r + r->pal, 3, 0x6200);
            break;
        }
    }
    if (mask & 4) {
        UnkScr_023418c4(r, 0xf);
    }
}
