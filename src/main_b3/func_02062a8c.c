#include <nitro/types.h>

typedef struct { s32 a; s32 b; s32 mode; s32 c; } UnkPair_02062a8c;
typedef struct { u8 pad0[8]; UnkPair_02062a8c sub; } UnkInfo_02062a8c;
typedef struct UnkWork_02062a8c {
    void (*fn)(struct UnkWork_02062a8c *w);
    u8 pad4[0x68];
    UnkInfo_02062a8c *info;  /* 0x6c */
} UnkWork_02062a8c;

void *func_0206041c(u8 a, u8 b);
void func_02062214(void *res);
void func_020617a4(void (*fn)(void));
void func_02061ee4(void);
void func_02062a38(UnkWork_02062a8c *w);

#pragma thumb on
void func_02062a8c(UnkWork_02062a8c *w) {
    u8 buf[0x30] = {0};
    UnkInfo_02062a8c *info = w->info;
    {
        UnkPair_02062a8c *q = &info->sub;
        if (q->c != 1)
            func_02062214(func_0206041c(q->a, q->b));
    }
    if (info->sub.mode == 2)
        func_020617a4(func_02061ee4);
    w->fn = func_02062a38;
}
#pragma thumb reset
