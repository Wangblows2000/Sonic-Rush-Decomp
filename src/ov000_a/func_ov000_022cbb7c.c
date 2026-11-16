#include <nitro/types.h>

typedef struct UnkStruct_022cbb7c_m {
    s32 m[9];
} UnkStruct_022cbb7c_m;

typedef struct UnkStruct_022cbb7c_2 {
    u8 pad[0x21c];
    u16 unk21C;
    u16 unk21E;
} UnkStruct_022cbb7c_2;

extern UnkStruct_022cbb7c_2 *gBoss1;
extern s16 data_0206e470[];
extern u8 data_ov000_02362bc4[];
extern u8 data_ov000_02362be4[];

s32 NNS_G3dGetResDictIdxByName(void *p, void *k);
void MTX_RotY33_(void *m, s32 s, s32 c);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);

void func_ov000_022cbb7c(u8 *p) {
    UnkStruct_022cbb7c_m m1;
    UnkStruct_022cbb7c_m m2;
    s32 r1 = NNS_G3dGetResDictIdxByName(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, data_ov000_02362bc4);
    s32 r2 = NNS_G3dGetResDictIdxByName(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, data_ov000_02362be4);
    s32 expect;
    if (*(u32 *)(p + 8) & 0x10)
        expect = p[0xae];
    else
        expect = -1;
    if (r1 == expect) {
        u16 a = gBoss1->unk21C;
        MTX_RotY33_(&m1, data_0206e470[((a & 0xffff) >> 4) * 2], data_0206e470[((a & 0xffff) >> 4) * 2 + 1]);
        NNS_G3dGeBufferOP_N(0x1a, &m1, 9);
        return;
    }
    if (*(u32 *)(p + 8) & 0x10)
        expect = p[0xae];
    else
        expect = -1;
    if (r2 == expect) {
        u16 a = gBoss1->unk21E;
        MTX_RotY33_(&m2, data_0206e470[((a & 0xffff) >> 4) * 2], data_0206e470[((a & 0xffff) >> 4) * 2 + 1]);
        NNS_G3dGeBufferOP_N(0x1a, &m2, 9);
    }
}
