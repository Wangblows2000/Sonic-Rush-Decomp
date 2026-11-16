#include <nitro/types.h>

typedef struct {
    u8 pad[4];
    u8 *unk4;
} UnkStruct_Res;

typedef struct {
    u8 pad[4];
    UnkStruct_Res *unk4;
    u32 unk8;
    u8 padC[0xAE - 0xC];
    u8 unkAE;
} UnkStruct_022f4d40;

extern u8 data_ov000_02363efc[];
extern u8 data_ov000_02363f0c[];
extern u8 data_ov000_02363f1c[];
extern u8 data_ov000_02363f2c[];
extern u8 data_ov000_02363f3c[];

s32 NNS_G3dGetResDictIdxByName(void *dict, void *name);
void NNS_G3dGeBufferOP_N(s32 a, s32 *b, s32 c);

void func_ov000_022f4d40(UnkStruct_022f4d40 *s) {
    s32 i0 = NNS_G3dGetResDictIdxByName(s->unk4->unk4 + 0x40, data_ov000_02363efc);
    s32 i1 = NNS_G3dGetResDictIdxByName(s->unk4->unk4 + 0x40, data_ov000_02363f0c);
    s32 i2 = NNS_G3dGetResDictIdxByName(s->unk4->unk4 + 0x40, data_ov000_02363f1c);
    s32 i3 = NNS_G3dGetResDictIdxByName(s->unk4->unk4 + 0x40, data_ov000_02363f2c);
    s32 i4 = NNS_G3dGetResDictIdxByName(s->unk4->unk4 + 0x40, data_ov000_02363f3c);
    s32 cur = (s->unk8 & 0x10) ? s->unkAE : -1;

    if (cur == i0) {
        s32 v = 0x1e;
        NNS_G3dGeBufferOP_N(0x13, &v, 1);
    } else if (cur == i1) {
        s32 v = 0x1d;
        NNS_G3dGeBufferOP_N(0x13, &v, 1);
    } else if (cur == i2) {
        s32 v = 0x1c;
        NNS_G3dGeBufferOP_N(0x13, &v, 1);
    } else if (cur == i3) {
        s32 v = 0x1b;
        NNS_G3dGeBufferOP_N(0x13, &v, 1);
    } else if (cur == i4) {
        s32 v = 0x1a;
        NNS_G3dGeBufferOP_N(0x13, &v, 1);
    }
}
