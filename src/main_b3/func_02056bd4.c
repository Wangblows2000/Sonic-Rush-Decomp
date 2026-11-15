#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02056bd4;
typedef struct UnkNode_02056bd4 UnkNode_02056bd4;
extern UnkNode_02056bd4 *data_02250438;
extern UnkNode_02056bd4 *data_02250434;

void func_02061fec(void);
s32 func_02056c8c(void *p);
u32 func_02056c80(void *p);
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
void func_02056af0(UnkRes_02056bd4 *res);

#pragma thumb on
void func_02056bd4(void *unused, UnkRes_02056bd4 *res) {
    u8 *p = (u8 *)res + res->chr;

    switch (func_02056c8c(p)) {
    case 0x10:
        func_02008648(p, (void *)0x0600c000);
        break;
    case 0x20:
        func_02008720(p, (void *)0x0600c000);
        break;
    case 0x30:
        func_020087f8(p, (void *)0x0600c000);
        break;
    default:
        func_02033a68(func_02033a54(p), (void *)0x0600c000, func_02056c80(p));
        break;
    }
    DC_StoreRange((void *)0x0600c000, func_02056c80(p));
    func_02035d04((u8 *)res + res->scr, 0, 0, 0x20, (res->flags & 0x1000) ? 1 : 0, 9, 0, 0, 0, 0x12, 0x20, 6);
    func_02056af0(res);
}
#pragma thumb reset
