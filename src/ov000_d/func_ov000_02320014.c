#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02320014_184;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02320014_4560;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x184 - 0x10];
    UnkStruct_02320014_184 *unk184;
} UnkStruct_02320014;

extern UnkStruct_02320014_4560 data_022c4560;

void func_ov000_02320344(UnkStruct_02320014 *w, u16 i);

void func_ov000_02320014(UnkStruct_02320014 *w) {
    s32 i;
    u32 fl;

    if (w->unkC == 0) {
        w->unkC = 1;
        if (w->unk184->unk2 == 0xec) {
            for (i = 0; i < 0xc; i++) {
                func_ov000_02320344(w, i);
            }
        } else {
            for (i = 0xd; i < 0x19; i++) {
                func_ov000_02320344(w, i);
            }
        }
    }
    fl = data_022c4560.unk20;
    if (fl & 0x100) {
        if (!(fl & 0xff)) {
            w->unk8 = 0xb6db6db6;
            if (w->unk184->unk2 == 0xee) {
                w->unk8 = ~w->unk8;
            }
        }
        fl = data_022c4560.unk20;
        if (!(fl & 0x80)) {
            if (!(fl & 0xf)) {
                w->unk8 <<= 1;
            }
        } else {
            if (!(fl & 3)) {
                w->unk8 <<= 1;
            }
        }
        if (!(w->unk8 & 2) || !(w->unk8 & 4)) {
            w->unk8 |= 1;
        }
    } else {
        if (!(fl & 0x20)) {
            w->unk8 = 0xffffffff;
        } else {
            w->unk8 = 0;
        }
        if (w->unk184->unk2 == 0xee) {
            w->unk8 = ~w->unk8;
        }
    }
}
