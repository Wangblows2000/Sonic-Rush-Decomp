#include <nitro/types.h>

typedef struct {
    u32 unk0;
    s32 numFrames;
    u8 pad8[0x18];
} UnkSeq_02040df8;

typedef struct {
    u32 unk0;
    u32 numSeq;
    u32 seqOffset;
} UnkBank_02040df8;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
    u32 seqIdx;
    s32 frame;
    u32 pad1c;
    s32 unk20;
    s32 unk24;
    UnkBank_02040df8 *bank;
} UnkAnim_02040df8;

#pragma thumb on
void KeyframeAnim_SetFrame(UnkAnim_02040df8 *a, s32 frame)
{
    s32 n;

    if (a->seqIdx >= a->bank->numSeq)
        n = 0;
    else
    {
        UnkSeq_02040df8 *s = (UnkSeq_02040df8 *)((u8 *)a->bank + a->bank->seqOffset);
        n = s[a->seqIdx].numFrames;
    }
    if (frame >= n * 0x1000) {
        if (n * 0x1000 > 0)
            frame = (n - 1) * 0x1000;
        else
            frame = 0;
    }
    a->flags &= ~0x40;
    a->frame = frame;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}
#pragma thumb reset
