#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_0231f230;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    UnkVec_0231f230 unk18;
    UnkVec_0231f230 unk24;
    UnkVec_0231f230 unk30;
    UnkVec_0231f230 unk3C;
} UnkStruct_0231f230_C;

typedef struct {
    u8 pad0[8];
    u8 unk8[0x4C - 8];
    u8 unk4C[0xFC - 0x4C];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkVec_0231f230 unk240;
    UnkVec_0231f230 unk24C;
    UnkVec_0231f230 unk258;
} UnkStruct_0231f230_G;

extern UnkStruct_0231f230_C data_ov000_0236a444;
extern UnkStruct_0231f230_G data_02084d38;
extern s16 data_0206e470[];

void func_02004988(s32 s, s32 c, s32 a, s32 b, s32 d, s32 e, s32 f, void *mtx);
void func_020044f4(UnkVec_0231f230 *pos, UnkVec_0231f230 *up, UnkVec_0231f230 *at, s32 a, void *mtx);
void VEC_Add(const UnkVec_0231f230 *a, const UnkVec_0231f230 *b, UnkVec_0231f230 *ab);
s32 func_0200225c(s32 a, s32 b);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline void LookAt_0231f230(UnkVec_0231f230 *pos, UnkVec_0231f230 *up, UnkVec_0231f230 *at) {
    data_02084d38.unk240 = *pos;
    data_02084d38.unk24C = *up;
    data_02084d38.unk258 = *at;
    func_020044f4(pos, up, at, 0, data_02084d38.unk4C);
    data_02084d38.unkFC &= ~0xe8;
}

void func_ov000_0231f230(void) {
    UnkVec_0231f230 pos;
    UnkVec_0231f230 at;

    func_02004988(SIN(data_ov000_0236a444.unk4), COS(data_ov000_0236a444.unk4), data_ov000_0236a444.unk10,
                  data_ov000_0236a444.unk8, data_ov000_0236a444.unkC, data_ov000_0236a444.unk14, 0,
                  data_02084d38.unk8);
    data_02084d38.unkFC &= ~0x50;
    data_ov000_0236a444.unk24.x = 0x80000;
    data_ov000_0236a444.unk24.y = -0x60000;
    data_ov000_0236a444.unk24.z = 0;
    VEC_Add(&data_ov000_0236a444.unk24, &data_ov000_0236a444.unk3C, &at);
    data_ov000_0236a444.unk18.x = 0x80000;
    data_ov000_0236a444.unk18.y = -0x60000;
    data_ov000_0236a444.unk18.z = func_0200225c(COS(data_ov000_0236a444.unk4), SIN(data_ov000_0236a444.unk4)) * 0x60;
    VEC_Add(&data_ov000_0236a444.unk18, &data_ov000_0236a444.unk3C, &pos);
    LookAt_0231f230(&pos, &data_ov000_0236a444.unk30, &at);
}
