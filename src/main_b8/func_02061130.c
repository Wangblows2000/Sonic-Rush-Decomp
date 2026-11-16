#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u32 flags;
} UnkDef_02061130;

typedef struct {
    UnkDef_02061130 *def;
    void **anim;
} UnkSlot_02061130;

typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[8];
    s32 unk10;
    u8 pad14[0xbc];
    UnkSlot_02061130 slot;
} UnkWork_02061130;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_02061130;

UnkRes_02061130 **TalkDemo_GetEntry(void);
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);
void Sound_PlayVoice(void *a, s32 b, s32 c);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);

#pragma thumb on
void func_02061130(UnkWork_02061130 *w) {
    UnkSlot_02061130 *s = &w->slot;
    UnkDef_02061130 *d;
    UnkRes_02061130 *r;
    s32 v;

    if (!(w->flags & 0x100))
        return;
    d = s->def;
    if (d->unk4 > w->unk10)
        return;
    r = *TalkDemo_GetEntry();
    if (r == NULL)
        return;
    v = r->unk4;
    if (*s->anim != NULL) {
        NNS_SndPlayerStopSeq(s->anim, 0);
        NNS_SndHandleReleaseSeq(s->anim);
    }
    if (d->flags & 1)
        Sound_PlayVoice(s->anim, v, d->unk0);
    else
        Sound_PlaySeqArc(s->anim, v, d->unk0);
    w->flags &= ~0x100;
}
#pragma thumb reset
