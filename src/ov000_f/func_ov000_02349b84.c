#include <nitro/types.h>

typedef struct UnkStruct_02349b84 UnkStruct_02349b84;

struct UnkStruct_02349b84 {
    u8 pad0[8];
    u32 unk8;
    u8 padC[0x2E - 0xC];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(UnkStruct_02349b84 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x368 - 0x354];
    s16 unk368;
    u8 pad36A[0x370 - 0x36A];
    s16 unk370;
    u8 pad372[0x384 - 0x372];
    u8 unk384[0x3C4 - 0x384];
    s16 unk3C4;
    s16 unk3C6;
    u8 pad3C8[0x3CF - 0x3C8];
    u8 unk3CF;
};

extern s16 data_0206e470[];
extern u32 gRandSeed;

#define SIN_02349b84(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

BOOL func_ov000_0232bb20(void);
void func_ov000_0234c4a4(UnkStruct_02349b84 *w, s32 state);
void func_ov000_0234a288(UnkStruct_02349b84 *w);
void func_ov000_02331a24(UnkStruct_02349b84 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);
void func_ov000_02331398(UnkStruct_02349b84 *w, s32 a, s32 b);
void func_ov000_02345830(UnkStruct_02349b84 *w, s32 a);

static inline u16 Rand_02349b84(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_02349b84(UnkStruct_02349b84 *w) {
    u8 ang = 0;
    s32 t;
    s32 a;

    if (!func_ov000_0232bb20()) {
        w->unk3CF = 0;
    }
    func_ov000_0234c4a4(w, 0x37);
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk32C |= 0x8000;
    w->unk68 = func_ov000_0234a288;
    w->unk30 = 0;
    w->unk32C &= ~1;
    w->unk32C |= 1;
    w->unk32C |= 8;
    if (w->unk324 != 1) {
        if (w->unk5C & 1) {
            ang -= 0x40;
        } else {
            ang += 0x40;
        }
        t = ((w->unk370 << 1) * SIN_02349b84((u16)(ang << 8)) << 4) >> 16;
        if ((t > 0 && w->unk2E < 0) || (t < 0 && w->unk2E > 0)) {
            w->unk2E = 0;
        }
        w->unk2E += t;
        ObjRect_SetAtkDefFlags(w->unk2CC, 0x13, 0);
        func_ov000_02331398(w, 0, 0);
        Sound_PlaySeqArcEx(w->unk384, 0, 0x10);
        Sound_PlaySeqArcEx(w->unk384, 0, (Rand_02349b84() & 3) + 0x1d);
        w->unk352 = 0xc;
    } else {
        w->unk3C6 = w->unk2E;
        w->unk2E = w->unk368 << 1;
        if (w->unk5C & 1) {
            w->unk2E = -w->unk2E;
        }
        a = w->unk3C6;
        if (a < 0) {
            a = -a;
        }
        if (a < 0x300) {
            w->unk3C6 = w->unk2E >> 3;
        }
        w->unk62 = 0x44;
        w->unk352 = 0xc;
        w->unk3C4 = 0xc;
        func_ov000_02331a24(w);
        Sound_PlaySeqArcEx(w->unk384, 0, 0x1c);
        Sound_PlaySeqArcEx(w->unk384, 0, (Rand_02349b84() & 3) + 0x26);
    }
    w->unk58 |= 0x4000;
    w->unk8 = 0;
    w->unk32A = 0;
    func_ov000_02345830(w, 200);
}
