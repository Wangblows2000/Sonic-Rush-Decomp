#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s16 alpha[32];
    u16 delay[32];
} UnkSub_020470f8;

typedef struct {
    u8 pad0[0x16c];
    UnkSub_020470f8 sub;
    u8 pad[0x344 - 0x16c - sizeof(UnkSub_020470f8)];
    u32 flags;
} UnkObj_020470f8;

void ClearDemo_Unk_020471ec(UnkObj_020470f8 *o, UnkSub_020470f8 *s);
void ClearDemo_Unk_0204714c(void *);

#pragma thumb on
void ClearDemo_Unk_020470f8(UnkObj_020470f8 *o) {
    UnkSub_020470f8 *s = &o->sub;
    s32 i;
    ClearDemo_Unk_020471ec(o, s);
    o->flags |= 0x701;
    for (i = 0; i < s->count; i++)
        s->alpha[i] = 0x1000;
    s->func = ClearDemo_Unk_0204714c;
}
#pragma thumb reset
