#include <nitro/types.h>

typedef struct {
    s32 a;
    s32 b;
} UnkSub_02062a38;

typedef struct {
    u8 pad0[0x1c];
    UnkSub_02062a38 sub;
} UnkEnt_02062a38;

typedef struct {
    void (*state)(void *);
    u8 pad4[0x64];
    void *unk68;
    u8 pad6c[8];
    s32 unk74;
} UnkWork_02062a38;

UnkEnt_02062a38 *TalkDemo_GetScriptPage(void *p, u8 i);
void TalkDemo_LoadImageByIndex(u8 a);
void TalkDemo_LoadImage22x10(void);
void TalkDemo_CreateDeferredCall(void (*f)(void));
void TalkDemo_FlipImage22x10(void);
void TalkDemoScene_State_LoadMessage(void *w);

#pragma thumb on
void TalkDemoScene_State_LoadImage22x10(UnkWork_02062a38 *w) {
    u8 buf[0x30] = {0};
    UnkEnt_02062a38 *e;
    UnkSub_02062a38 *s;

    e = TalkDemo_GetScriptPage(w->unk68, w->unk74);
    s = &e->sub;
    TalkDemo_LoadImageByIndex(s->a);
    TalkDemo_LoadImage22x10();
    if (s->b == 2)
        TalkDemo_CreateDeferredCall(TalkDemo_FlipImage22x10);
    w->state = TalkDemoScene_State_LoadMessage;
}
#pragma thumb reset
