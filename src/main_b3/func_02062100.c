#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02062100;
typedef struct UnkNode_02062100 UnkNode_02062100;
extern UnkNode_02062100 *data_02250438;
extern UnkNode_02062100 *data_02250434;

void func_02061fcc(void);
s32 func_020620f4(void *p);
u32 func_020620e8(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_02060e70(s32 a);
void func_02035704(void *p, s32 a, s32 b, u32 c);
void func_020356bc(void *p, u32 a, u32 b);
void func_02035754(void);
void func_02035d04(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_02061840(UnkRes_02062100 *res);

#pragma thumb on
static inline void LoadPal_02062100(u8 *src) {
    UnkNode_02062100 *s0 = data_02250438;
    UnkNode_02062100 *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_02035704(src + 0x1c4, 0x10, 0, 0x050001c0);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

void func_02062100(UnkRes_02062100 *res) {
    u8 *p;

    if (res == NULL) {
        func_02061fcc();
        return;
    }
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    p = (u8 *)res + res->chr;
    switch (func_020620f4(p)) {
    case 0x10:
        func_02008648(p, (void *)0x06004000);
        break;
    case 0x20:
        func_02008720(p, (void *)0x06004000);
        break;
    case 0x30:
        func_020087f8(p, (void *)0x06004000);
        break;
    default:
        func_02033a68(func_02033a54(p), (void *)0x06004000, func_020620e8(p));
        break;
    }
    DC_StoreRange((void *)0x06004000, func_020620e8(p));
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    LoadPal_02062100((u8 *)res + res->pal);
    func_02035d04((u8 *)res + res->scr, 0, 0, 0x16, (res->flags & 0x1000) ? 1 : 0, 1, 0, 6, 0, 0, 0x16, 10);
    func_02061840(res);
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
}
#pragma thumb reset
