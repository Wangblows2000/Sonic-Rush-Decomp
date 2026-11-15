#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 unk4;
    u8 pad8[0x98];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
} UnkObj_0204625c;
typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkWork_0204625c;

extern u32 data_0208fe70;
void func_0204622c(void);

#pragma thumb on
static inline u16 Rand_0204625c(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_0204625c(UnkWork_0204625c *w, UnkObj_0204625c *o) {
    s32 amp;

    o->unk4++;
    amp = (0x10 - o->unk4) * 0x500;
    if (amp != 0) {
        o->unkA4 = (Rand_0204625c() % amp + 0x50000) - amp / 2;
        o->unkA8 = (Rand_0204625c() % amp + 0x8a000) - amp / 2;
    } else {
        o->unkA4 = 0x50000;
        o->unkA8 = 0x8a000;
    }
    if (o->unk4 >= 0x10) {
        o->unkA4 = 0x50000;
        o->unkA8 = 0x8a000;
        o->fn = func_0204622c;
        o->unk4 = 0;
        w->flags |= 0x800000;
    }
}
#pragma thumb reset
