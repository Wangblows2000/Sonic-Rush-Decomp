#include <nitro/types.h>

typedef struct {
    void *def;
    void **anim;
} UnkSlot_0206267c;

typedef struct {
    void (*state)(void *);
    u8 pad4[8];
    s32 unkc;
    u8 pad10[0x70];
    u16 unk80;
    u8 pad82[0x4e];
    UnkSlot_0206267c slot;
} UnkWork_0206267c;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_0206267c;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} UnkTbl_0206267c;

extern UnkTbl_0206267c data_02076e58;

void TalkDemoRocks_Create(u16 a);
UnkRes_0206267c **TalkDemo_GetEntry(void);
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);
void TalkDemoScene_State_RockPageWait(void *w);

#pragma thumb on
void TalkDemoScene_State_RockPageSpawn(UnkWork_0206267c *w) {
    UnkSlot_0206267c *s;
    UnkRes_0206267c *r;
    s32 v;

    TalkDemoRocks_Create(w->unk80);
    s = &w->slot;
    r = *TalkDemo_GetEntry();
    if (r != NULL) {
        v = r->unk4;
        if (*s->anim != NULL) {
            NNS_SndPlayerStopSeq(s->anim, 0);
            NNS_SndHandleReleaseSeq(s->anim);
        }
        Sound_PlaySeqArc(s->anim, v, data_02076e58.unk24);
    }
    w->unkc = 1;
    w->state = TalkDemoScene_State_RockPageWait;
}
#pragma thumb reset
