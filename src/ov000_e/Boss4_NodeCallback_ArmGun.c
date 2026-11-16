#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 *unk4;
} UnkStruct_022de51c_R;

typedef struct {
    u8 pad0[4];
    UnkStruct_022de51c_R *unk4;
    u32 unk8;
    u8 padC[0x24 - 0xC];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
    u8 pad93[0xAE - 0x93];
    u8 unkAE;
} UnkStruct_022de51c;

typedef struct {
    u8 pad0[0xAF4];
    u32 unkAF4;
    u8 padAF8[0xB58 - 0xAF8];
    u8 unkB58[4];
} UnkStruct_022de51c_M;

extern UnkStruct_022de51c_M *gBoss4;
extern u8 data_ov000_02363610[];

s32 NNS_G3dGetResDictIdxByName(void *dict, void *name);
void Boss4_CaptureNodeMatrix(void *a, UnkStruct_022de51c *o, s32 b);

void Boss4_NodeCallback_ArmGun(UnkStruct_022de51c *o) {
    UnkStruct_022de51c_M *m = gBoss4;
    s32 id;

    if (o->unk8 & 0x10)
        id = o->unkAE;
    else
        id = -1;
    if (id != NNS_G3dGetResDictIdxByName(o->unk4->unk4 + 0x40, data_ov000_02363610))
        return;
    Boss4_CaptureNodeMatrix(m->unkB58, o, 0x4);
    if ((m->unkAF4 & 0x4) == 0x4) {
        o->unk24 = 0;
        o->unk92 = 0;
    }
}
