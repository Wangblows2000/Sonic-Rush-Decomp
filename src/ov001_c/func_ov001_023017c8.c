#include <nitro/types.h>

typedef struct UnkStruct_023017c8_g {
    u8 pad0[0x18];
    u16 keys;
    u8 pad1A[0x3c - 0x1a];
    s32 unk3C;
} UnkStruct_023017c8_g;

typedef struct UnkStruct_023017c8_t {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_023017c8_t;

typedef struct UnkStruct_023017c8 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x3c - 0x18];
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
    u8 pad42[0x50 - 0x42];
    s32 unk50;
    s32 unk54;
    u8 pad58[4];
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x9c - 0x64];
    void *unk9C;
    void *unkA0;
    u8 padA4[0xb0 - 0xa4];
    u8 unkB0[0xe8 - 0xb0];
    u8 unkE8[4];
    void (*unkEC)(void);
    u8 padF0[0x1c8 - 0xf0];
    s16 unk1C8;
    u8 pad1CA[2];
    s32 unk1CC;
} UnkStruct_023017c8;

extern UnkStruct_023017c8_t data_ov001_0231da50;
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

UnkStruct_023017c8_g *ExtraBoss_GetWork(void);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void ExtraBossPlayer_ChangeState(UnkStruct_023017c8 *p, s32 a);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_023019f8(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_023017c8(UnkStruct_023017c8 *p) {
    u16 keys = ExtraBoss_GetWork()->keys;
    u16 ang;
    s32 idx;

    p->flags &= ~0x700;
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlaySeqArc(0, 0, 9);
    else
        Sound_PlaySeqArc(0, 0, 0x13);
    if (keys & 0x20) {
        ExtraBossPlayer_ChangeState(p, 4);
        if (keys & 0x40) {
            p->unk40 = 0xe000;
            ang = 0xa000;
        } else if (keys & 0x80) {
            p->unk40 = 0x2000;
            ang = 0x6000;
        } else {
            p->unk40 = 0;
            ang = 0x8000;
        }
    } else {
        ExtraBossPlayer_ChangeState(p, 3);
        if (keys & 0x40) {
            if (keys & 0x10) {
                p->unk40 = 0x2000;
                ang = 0xe000;
            } else {
                p->unk40 = 0x4000;
                ang = 0xc000;
            }
        } else if (keys & 0x80) {
            if (keys & 0x10) {
                p->unk40 = 0xe000;
                ang = 0x2000;
            } else {
                p->unk40 = 0xc000;
                ang = 0x4000;
            }
        } else {
            ang = 0;
            p->unk40 = 0;
        }
    }
    idx = (s32)(ang & 0xffff) >> 4;
    p->unk50 = FX_Mul(data_ov001_0231da50.unk14, COS_IDX(idx));
    p->unk54 = FX_Mul(data_ov001_0231da50.unk14, SIN_IDX(idx));
    p->unk5C = FX_Mul(data_ov001_0231da50.unk10, COS_IDX(idx));
    p->unk60 = FX_Mul(data_ov001_0231da50.unk10, SIN_IDX(idx));
    p->flags |= 0x300;
    p->unk1C8 = p->unk3C;
    func_ov001_022fe8d4(p->unkE8, p, 2, 1, 0x8000, 0x8000, 0x8000, 0, -0x8000, 0, 0);
    p->unkEC = func_ov001_023019f8;
    p->unkA0 = p->unkE8;
    p->unk1CC = 2;
    p->unk9C = p->unkB0;
}
