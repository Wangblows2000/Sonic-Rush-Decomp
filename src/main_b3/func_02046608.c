#include <nitro/types.h>

typedef struct { s32 x; s32 y; } UnkVec_02046608;
typedef struct {
    void (*fn)(void);
    s32 unk4;
    UnkVec_02046608 pos[6]; /* 0x8 */
    s16 unk38[6];           /* 0x38 */
    u16 unk44[6];           /* 0x44 */
    u16 *unk50;             /* 0x50 */
} UnkObj_02046608;
typedef struct {
    u8 pad0[0x1630];
    void *unk1630;          /* 0x1630 */
} UnkWork_02046608;

void func_02036754(void *a, void *b, s32 c);
void func_020465b8(void);

#pragma thumb on
static inline s32 FX_Mul_02046608(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_02046608(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_02046608(to - v, t);
    } while (n--);
    return v;
}

void func_02046608(UnkWork_02046608 *w, UnkObj_02046608 *o) {
    s32 n;
    s32 i;

    o->unk4++;
    n = o->unk4 / 30 + 1;
    if (n > 5)
        n = 5;
    for (i = 0; i < n; i++) {
        o->unk38[i] += 0x100;
        if (o->unk38[i] > 0x1000) {
            o->unk38[i] = 0x1000;
            if (o->unk50[i] != 0xffff && o->unk44[i] == 0) {
                func_02036754(NULL, w->unk1630, 0);
                o->unk44[i] = 1;
            }
        }
        o->pos[i].x = Lerp_02046608(0, 0xc8000, o->unk38[i], 5) - 0x72000;
    }
    if (n == 5 && o->unk38[n - 1] == 0x1000) {
        o->fn = func_020465b8;
        o->unk4 = 0;
    }
}
#pragma thumb reset
