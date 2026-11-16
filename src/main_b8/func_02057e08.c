#include <nitro/types.h>

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_02057e08;

typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0x24];
    UnkRes_02057e08 **res;
    u8 pad30[4];
    void *anim;
} UnkObj_02057e08;

extern s16 data_0207613c[];

void func_020575b4(UnkObj_02057e08 *o);
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);

#pragma thumb on
void func_02057e08(UnkObj_02057e08 *o, s32 n) {
    if (o->flags & 0x10000000)
        return;
    switch (n) {
    case 4:
        func_020575b4(o);
    case 1:
    case 0xd:
        if (o->anim != NULL) {
            NNS_SndPlayerStopSeq(&o->anim, 0);
            NNS_SndHandleReleaseSeq(&o->anim);
        }
        Sound_PlaySeqArc(&o->anim, (*o->res)->unk4, data_0207613c[n]);
        return;
    default:
        Sound_PlaySeqArc(NULL, (*o->res)->unk4, data_0207613c[n]);
        return;
    }
}
#pragma thumb reset
