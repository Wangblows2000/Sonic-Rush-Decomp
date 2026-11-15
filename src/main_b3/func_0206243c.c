#include <nitro/types.h>

typedef struct { s32 a; s32 b; } UnkPair_0206243c;
typedef struct { u8 pad0[8]; UnkPair_0206243c sub; u8 pad10[0x20]; s32 unk30; } UnkInfo_0206243c;
typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_0206243c;
typedef struct UnkWork_0206243c {
    void (*fn)(struct UnkWork_0206243c *w);
    u8 pad4[0x64];
    s32 unk68;
    UnkInfo_0206243c *info;  /* 0x6c */
    u8 pad70[4];
    s32 unk74;
    u8 pad78[6];
    u16 unk7E;
    u16 unk80;
} UnkWork_0206243c;
typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    s32 unk28;
    u16 unk2C;
    u16 unk2E;
    u16 unk30;
    u8 pad32[0xe];
} UnkParam_0206243c;

typedef struct UnkNode_0206243c UnkNode_0206243c;
extern UnkNode_0206243c *data_02250438;
extern UnkNode_0206243c *data_02250434;

UnkInfo_0206243c *func_02060470(s32 a, u8 b);
UnkRes_0206243c *func_0206041c(u8 a, u8 b);
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
void func_02061840(UnkRes_0206243c *res);
void func_02061a08(UnkParam_0206243c *p);
void func_02062404(UnkWork_0206243c *w);

#pragma thumb on
void func_0206243c(UnkWork_0206243c *w) {
    UnkInfo_0206243c *info;
    UnkRes_0206243c *res;
    u8 *p;

    w->info = func_02060470(w->unk68, w->unk74);
    info = w->info;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x4404;
    {
        u8 buf[0x30] = {0};
    }
    { UnkPair_0206243c *q = &info->sub; res = func_0206041c(q->a, q->b); }
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
    p = (u8 *)res + res->pal;
    if (!(func_02060e70(0) & 1)) {
        UnkNode_0206243c *s0 = data_02250438;
        UnkNode_0206243c *s1 = data_02250434;
        data_02250438 = NULL;
        data_02250434 = NULL;
        func_02035704(p + 4, 0xf0, 0, 0x05000000);
        func_02035754();
        data_02250438 = s0;
        data_02250434 = s1;
    } else {
        UnkNode_0206243c *s0 = data_02250438;
        UnkNode_0206243c *s1 = data_02250434;
        data_02250438 = NULL;
        data_02250434 = NULL;
        func_020356bc(p, 1, 0x4000);
        func_02035754();
        data_02250438 = s0;
        data_02250434 = s1;
    }
    func_02035d04((u8 *)res + res->scr, 0, 0, 0x20, (res->flags & 0x1000) ? 1 : 0, 9, 0, 4, 0, 0, 0x20, 7);
    func_02061840(res);
    {
        UnkParam_0206243c prm = {0};
        prm.unk2C = 0x100;
        prm.unk2E = 0x100;
        prm.unk30 = 0;
        prm.unk24 = 0;
        prm.unk28 = 0x4400;
        func_02061a08(&prm);
    }
    w->unk7E = 0;
    w->unk80 = info->unk30;
    w->fn = func_02062404;
}
#pragma thumb reset
