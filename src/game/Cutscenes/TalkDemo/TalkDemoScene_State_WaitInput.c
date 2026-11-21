#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 pad4;
    s32 unk8;
} UnkSlot_02062884;

typedef struct {
    void (*state)(void *);
    u8 pad4[0xcc];
    UnkSlot_02062884 slot;
} UnkWork_02062884;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_02062884;

void TalkDemoScene_DrawPrompt(UnkWork_02062884 *w);
u32 TalkDemo_GetFlags(s32 a);
UnkRes_02062884 **TalkDemo_GetEntry(void);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);
void TalkDemoScene_State_StartBg1Exit(void *w);

#pragma thumb on
void TalkDemoScene_State_WaitInput(UnkWork_02062884 *w) {
    TalkDemoScene_DrawPrompt(w);
    if (TalkDemo_GetFlags(3) & 0x50) {
        UnkSlot_02062884 *s = &w->slot;
        UnkRes_02062884 *r = *TalkDemo_GetEntry();
        if (r != NULL)
            Sound_PlaySeqArc(NULL, r->unk4, s->unk8);
        w->state = TalkDemoScene_State_StartBg1Exit;
    }
}
#pragma thumb reset
