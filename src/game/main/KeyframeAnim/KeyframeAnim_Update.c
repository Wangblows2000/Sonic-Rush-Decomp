#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 frames;
    u8 pad8[0x18];
} UnkSeq_02040d14;

typedef struct {
    u8 pad0[4];
    u32 count;
    u32 offset;
} UnkHdr_02040d14;

typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    s32 unk8;
    s16 unkc;
    u8 pade[2];
    u32 flags;
    u32 seq;
    s32 frame;
    s32 speed;
    s32 unk20;
    s32 unk24;
    UnkHdr_02040d14 *hdr;
} UnkAnim_02040d14;

typedef struct {
    s32 x;
    s32 y;
} UnkPos_02040d14;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} UnkScale_02040d14;

void KeyframeAnim_EvalPosition(UnkAnim_02040d14 *a, UnkSeq_02040d14 *seq, UnkPos_02040d14 *out);
void KeyframeAnim_EvalScaleRotation(UnkAnim_02040d14 *a, UnkSeq_02040d14 *seq, UnkScale_02040d14 *out);

#pragma thumb on
void KeyframeAnim_Update(UnkAnim_02040d14 *a) {
    UnkPos_02040d14 pos;
    UnkScale_02040d14 sc;
    UnkSeq_02040d14 *seq;
    UnkHdr_02040d14 *h;
    s32 max;
    u32 idx;
    if ((a->flags & 1) && !(a->flags & 0x20))
        return;
    idx = a->seq;
    h = a->hdr;
    if (idx >= h->count)
        return;
    seq = (UnkSeq_02040d14 *)((u8 *)h + h->offset) + idx;
    max = (seq->frames - 1) << 12;
    KeyframeAnim_EvalPosition(a, seq, &pos);
    a->x = (pos.x + 0x800) >> 12;
    a->y = (pos.y + 0x800) >> 12;
    KeyframeAnim_EvalScaleRotation(a, seq, &sc);
    a->unk4 = sc.a;
    a->unk8 = sc.b;
    a->unkc = sc.c;
    if (!(a->flags & 1)) {
        if (a->flags & 8) {
            a->frame -= a->speed;
            if (a->frame < 0) {
                if (a->flags & 2) {
                    a->frame = max;
                } else {
                    a->flags |= 0x41;
                    a->frame = 0;
                }
            }
        } else {
            a->frame += a->speed;
            if (a->frame > max && !(a->flags & 2)) {
                a->flags |= 0x41;
                a->frame = max;
            }
            if (a->frame < 0)
                a->frame = 0;
        }
    }
    a->flags &= ~0x20;
}
#pragma thumb reset
