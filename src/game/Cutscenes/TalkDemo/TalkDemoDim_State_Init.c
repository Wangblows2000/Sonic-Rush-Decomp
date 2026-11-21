#include <nitro/types.h>

typedef struct {
    u16 bg0a : 1;
    u16 bg1a : 1;
    u16 bg2a : 1;
    u16 bg3a : 1;
    u16 obja : 1;
    u16 bda : 1;
    u16 effect : 2;
    u16 bg0b : 1;
    u16 bg1b : 1;
    u16 bg2b : 1;
    u16 bg3b : 1;
    u16 objb : 1;
    u16 bdb : 1;
    u16 pad : 2;
    u16 eva : 5;
    u16 pad2 : 3;
    u16 evb : 5;
    u16 pad3 : 3;
    u16 evy;
} UnkBlend_0205f920;

typedef struct {
    void (*state)(void *);
    u16 unk4;
} UnkTask_0205f920;

extern UnkBlend_0205f920 data_0208924c;

u32 TalkDemo_GetFlags(s32 a);
void TalkDemoDim_State_FadeIn(void *t);

#pragma thumb on
void TalkDemoDim_State_Init(UnkTask_0205f920 *t) {
    UnkBlend_0205f920 *b = &data_0208924c;

    if (TalkDemo_GetFlags(0) & 1) {
        b->bg0a = 0;
        b->bg1a = 0;
        b->bg2a = 0;
        b->bg3a = 1;
        b->obja = 1;
        b->bda = 0;
        b->effect = 3;
        b->bg0b = 0;
        b->bg1b = 0;
        b->bg2b = 0;
        b->bg3b = 1;
        b->objb = 0;
        b->bdb = 0;
        b->eva = 0x10;
        b->evb = 0;
        b->evy = 0;
    } else {
        b->bg0a = 1;
        b->bg1a = 0;
        b->bg2a = 0;
        b->bg3a = 0;
        b->obja = 1;
        b->bda = 0;
        b->effect = 3;
        b->bg0b = 1;
        b->bg1b = 0;
        b->bg2b = 0;
        b->bg3b = 0;
        b->objb = 0;
        b->bdb = 0;
        b->eva = 0x10;
        b->evb = 0;
        b->evy = 0;
    }
    t->unk4 = 0;
    t->state = (void (*)(void *))TalkDemoDim_State_FadeIn;
}
#pragma thumb reset
