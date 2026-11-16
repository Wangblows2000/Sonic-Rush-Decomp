#include <nitro/types.h>

typedef struct {
    s32 a;
    s32 b;
} UnkPair_02045528;

typedef struct {
    void (*func)(void *);
    s32 timer;
    UnkPair_02045528 pairs[6];
    s16 scale[5];
    u8 pad42[0x42];
    s16 digits[6];
    s16 target[6];
} UnkSub_02045528;

typedef struct {
    u8 pad0[0x274];
    UnkSub_02045528 sub;
    u8 pad[0x344 - 0x274 - sizeof(UnkSub_02045528)];
    u32 flags;
    u8 pad348[0x1630 - 0x348];
    void *unk1630;
    u8 unk1634[4];
} UnkObj_02045528;

void func_02045768(void *);
void Sound_PlaySeqArc(void *a, void *b, s32 c);

#pragma thumb on
void func_02045528(UnkObj_02045528 *o) {
    s32 i;
    UnkSub_02045528 *s = &o->sub;
    s->timer = 0;
    o->flags |= 0x7000;
    for (i = 0; i < 3; i++) {
        s->pairs[i].a = 0x56000;
        s->scale[i] = 0x1000;
    }
    s->func = func_02045768;
    s->timer = 0x96;
    Sound_PlaySeqArc(o->unk1634, o->unk1630, 2);
}
#pragma thumb reset
