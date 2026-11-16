#include <nitro/types.h>

typedef struct UnkStruct_02301a88_w {
    u32 flags;
    u8 pad4[0xc];
    s32 unk10;
} UnkStruct_02301a88_w;

typedef struct UnkStruct_02301a88_o {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0xa0 - 0x20];
    s32 unkA0;
    u8 padA4[0x1c8 - 0xa4];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_02301a88_w *work;
} UnkStruct_02301a88_o;

typedef struct UnkStruct_02301a88_h {
    UnkStruct_02301a88_o *owner;
    u8 pad4[4];
    u32 flags;
} UnkStruct_02301a88_h;

typedef struct UnkStruct_02301a88_c {
    u8 pad0[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_02301a88_c;

UnkStruct_02301a88_c *ExtraBoss_GetWork(void);
void ExtraBoss_ScatterPlayerRings(s32 x, s32 y, s32 n);
void ExtraBossPlayer_ChangeState(UnkStruct_02301a88_o *o, s32 state);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

void ExtraBoss_PlayerOnHurt(UnkStruct_02301a88_h *self, UnkStruct_02301a88_h *other) {
    UnkStruct_02301a88_o *o = other->owner;
    UnkStruct_02301a88_w *w = o->work;

    if (self->flags & 0x10) {
        ExtraBoss_ScatterPlayerRings(o->x, o->y, ExtraBoss_GetWork()->unk40 <= 2 ? 5 : 10);
        Sound_PlaySeqArc(0, 0, 5);
    }
    if (self->flags & 0x20) {
        ExtraBossPlayer_ChangeState(o, 0xc);
        return;
    }
    if (self->flags & 0x40)
        return;
    other->flags |= 0x200;
    o->unkA0 = 0;
    ExtraBossPlayer_ChangeState(o, 5);
    o->unk1C8 = 0x3c;
    w->flags |= 2;
    w->unk10 = 0xb4;
    w->flags &= ~4;
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlayVoice(0, 0, 0x21);
    else
        Sound_PlayVoice(0, 0, 0x2a);
}
