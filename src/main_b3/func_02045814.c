#include <nitro/types.h>

typedef struct { s32 x; s32 y; } UnkVec_02045814;
typedef struct UnkObj_02045814 {
    void (*fn)(void);
    s32 unk4;
    UnkVec_02045814 pos[6]; /* 0x8 */
    s16 unk38[6];           /* 0x38 */
    u16 unk44[3];           /* 0x44 */
} UnkObj_02045814;
typedef struct {
    u8 pad0[0x344];
    u32 flags;              /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;          /* 0x1630 */
    u8 unk1634[4];          /* 0x1634 */
} UnkWork_02045814;

void Sound_PlaySeqArc(void *a, void *b, s32 c);
void func_02045768(void);

#pragma thumb on
static inline s32 FX_Mul_02045814(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_02045814(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_02045814(to - v, t);
    } while (n--);
    return v;
}

void func_02045814(UnkWork_02045814 *w, UnkObj_02045814 *o) {
    s32 n;
    s32 i;

    o->unk4++;
    n = o->unk4 / 30 + 1;
    if (n > 3)
        n = 3;
    for (i = 0; i < n; i++) {
        o->unk38[i] += 0x88;
        if (o->unk38[i] > 0x1000) {
            o->unk38[i] = 0x1000;
            if (o->unk44[i] == 0) {
                Sound_PlaySeqArc(NULL, w->unk1630, 0);
                o->unk44[i] = 1;
            }
        }
        o->pos[i].x = Lerp_02045814(0, 0xc8000, o->unk38[i], 5) - 0x72000;
    }
    if (n == 3 && o->unk38[n - 1] == 0x1000) {
        o->fn = func_02045768;
        o->unk4 = 0x96;
        Sound_PlaySeqArc(w->unk1634, w->unk1630, 2);
        w->flags |= 0x400000;
    }
}
#pragma thumb reset
