#include <nitro/types.h>

typedef struct {
    s16 acc;
    s16 max;
    s16 dec;
} UnkStruct_02347190_Prm;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x32 - 0x1C];
    s16 unk32;
    u8 pad34[0x32C - 0x34];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x356 - 0x334];
    UnkStruct_02347190_Prm prm;
    u8 pad35C[0x364 - 0x35C];
    s16 unk364;
    UnkStruct_02347190_Prm alt;
    u8 pad36C[0x420 - 0x36C];
    u16 unk420;
} UnkStruct_02347190;

s16 func_02068df8(s32 v, s32 a, s32 b);
s16 func_02068db4(s32 v, s32 a);

void func_ov000_02347190(UnkStruct_02347190 *w) {
    s32 acc;
    s32 max;
    s32 dec;

    if (!(w->unk330 & 0x100))
        return;
    if (w->unk18 >= 0) {
        w->unk18 = 0;
        w->unk32 = 0;
    }
    if (w->unk18 <= -0x2800) {
        w->unk18 = -0x2800;
        w->unk32 = 0;
    }
    acc = (s16)(w->prm.acc << 1);
    max = w->prm.max;
    dec = w->prm.dec;
    if (w->unk32C & 0x100)
        max = w->unk364;
    if (w->unk32C & 0x80) {
        acc = w->alt.acc;
        max = w->alt.max;
        dec = w->alt.dec;
    }
    if (w->unk420 & 0xC0) {
        if (w->unk420 & 0x80) {
            if (w->unk32 < 0)
                w->unk32 = func_02068db4(w->unk32, dec);
            w->unk32 = func_02068df8(w->unk32, acc, max);
        } else {
            if (w->unk32 > 0)
                w->unk32 = func_02068db4(w->unk32, dec);
            w->unk32 = func_02068df8(w->unk32, -acc, max);
        }
    } else {
        w->unk32 = func_02068db4(w->unk32, dec);
    }
}
