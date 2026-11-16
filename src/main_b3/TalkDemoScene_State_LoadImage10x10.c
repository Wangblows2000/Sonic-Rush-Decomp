#include <nitro/types.h>

typedef struct { s32 a; s32 b; s32 mode; s32 c; } UnkPair_02062a8c;
typedef struct { u8 pad0[8]; UnkPair_02062a8c sub; } UnkInfo_02062a8c;
typedef struct UnkWork_02062a8c {
    void (*fn)(struct UnkWork_02062a8c *w);
    u8 pad4[0x68];
    UnkInfo_02062a8c *info;  /* 0x6c */
} UnkWork_02062a8c;

void *TalkDemo_LoadImageByPair(u8 a, u8 b);
void TalkDemo_LoadImage10x10(void *res);
void TalkDemo_CreateDeferredCall(void (*fn)(void));
void TalkDemo_FlipImage10x10(void);
void TalkDemoScene_State_LoadImage22x10(UnkWork_02062a8c *w);

#pragma thumb on
void TalkDemoScene_State_LoadImage10x10(UnkWork_02062a8c *w) {
    u8 buf[0x30] = {0};
    UnkInfo_02062a8c *info = w->info;
    {
        UnkPair_02062a8c *q = &info->sub;
        if (q->c != 1)
            TalkDemo_LoadImage10x10(TalkDemo_LoadImageByPair(q->a, q->b));
    }
    if (info->sub.mode == 2)
        TalkDemo_CreateDeferredCall(TalkDemo_FlipImage10x10);
    w->fn = TalkDemoScene_State_LoadImage22x10;
}
#pragma thumb reset
