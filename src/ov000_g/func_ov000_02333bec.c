#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    u8 pad30[2];
    s16 unk32;
    u8 pad34[0x13E - 0x34];
    u8 unk13E;
    u8 unk13F;
} UnkStruct_02333bec;

extern s16 data_0206e470[];

UnkStruct_02333bec *func_ov000_02333cf8(s32 a, s32 b, s32 c, u16 d);
void func_ov000_022c5e80(s32 a, s32 b, s32 x, s32 *ox, s32 *oz, u16 *ang);

static inline s32 FX_MUL(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}
#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

UnkStruct_02333bec *func_ov000_02333bec(s32 a, s32 b, s32 c, s32 d, s32 e, u16 f) {
    u16 ang;
    s32 ox;
    s32 oz;
    s32 v;
    UnkStruct_02333bec *o = func_ov000_02333cf8(a, b, c, f);

    if (o == NULL)
        return NULL;
    o->unk13E = 0x14;
    o->unk13F = 7;
    func_ov000_022c5e80(d, e, o->unk10 << 4, &ox, &oz, &ang);
    o->unk10 = ox >> 4;
    o->unk18 = oz >> 4;
    v = o->unk2E << 4;
    o->unk2E = FX_MUL(v, COS(ang)) >> 4;
    o->unk32 = FX_MUL(v, SIN(ang)) >> 4;
    return o;
}
