#include <nitro/types.h>

typedef struct {
    s32 a;
    s32 b;
} UnkPair_020461b4;

typedef struct {
    void (*func)(void *);
    s32 timer;
    UnkPair_020461b4 pairs[6];
    s16 scale[5];
    u8 pad42[0x42];
    s16 digits[6];
    s16 target[6];
} UnkSub_020461b4;

typedef struct {
    u8 pad0[0x274];
    UnkSub_020461b4 sub;
    u8 pad[0x344 - 0x274 - sizeof(UnkSub_020461b4)];
    u32 flags;
    u8 pad348[0x1630 - 0x348];
    void *unk1630;
    u8 unk1634[4];
} UnkObj_020461b4;

void func_020463bc(void *);
void NNS_SndPlayerStopSeq(void *p, s32 a);

#pragma thumb on
void func_020461b4(UnkObj_020461b4 *o) {
    s32 i;
    UnkSub_020461b4 *s = &o->sub;
    s->timer = 0;
    o->flags |= 0xf000;
    for (i = 0; i <= 4; i++) {
        s->pairs[i].a = 0x56000;
        s->scale[i] = 0x1000;
    }
    for (i = 0; i < 6; i++)
        s->digits[i] = s->target[i];
    s->func = func_020463bc;
    NNS_SndPlayerStopSeq(o->unk1634, 0);
}
#pragma thumb reset
