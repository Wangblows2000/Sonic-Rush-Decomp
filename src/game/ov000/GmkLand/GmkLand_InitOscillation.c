#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
} UnkStruct_0230f0bc_sub;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x1E0 - 0x24];
    UnkStruct_0230f0bc_sub *unk1E0;
} UnkStruct_0230f0bc;

extern s16 data_0206e470[];

void GmkLand_InitOscillation(UnkStruct_0230f0bc *s) {
    s32 w;
    s32 pos;
    s32 base;
    u16 ang;

    s->unk1C = (s->unk10 >> 8) + s->unk1E0->unk6 + (s->unk1E0->unk8 >> 1);
    s->unk20 = (s->unk14 >> 8) + s->unk1E0->unk7 + (s->unk1E0->unk9 >> 1);
    if (s->unk1E0->unk9 == 0 && s->unk1E0->unk8 == 0) {
        return;
    }
    if (s->unk1E0->unk9 < s->unk1E0->unk8) {
        w = s->unk1E0->unk8 >> 1;
        pos = s->unk10 >> 8;
        base = s->unk1C;
    } else {
        w = s->unk1E0->unk9 >> 1;
        pos = s->unk14 >> 8;
        base = s->unk20;
    }
    ang = 0x300;
    do {
        if (base + ((s32)(w * data_0206e470[((s32)((u16)(ang << 6) & 0xffff) >> 4) * 2]) >> 12) > pos) {
            break;
        }
        ang -= 4;
    } while (ang > 0x100);
    s->unkC = ang;
    s->unkC -= (s16)(((s->unk1E0->unk4 & 0x18) >> 3) << 8);
    s->unkC &= 0x3fff;
}
