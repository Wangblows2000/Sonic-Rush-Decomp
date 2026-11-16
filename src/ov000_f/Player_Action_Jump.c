#include <nitro/types.h>

typedef struct UnkStruct_0234ac58 UnkStruct_0234ac58;

typedef struct {
    u8 pad0[0x114];
    s32 unk114;
} UnkStruct_0234ac58_Spr;

struct UnkStruct_0234ac58 {
    u8 pad0;
    u8 unk1;
    u8 pad2[8 - 2];
    u32 unk8;
    u32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 y;
    s32 z;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(UnkStruct_0234ac58 *w);
    u8 pad6C[0x124 - 0x6C];
    UnkStruct_0234ac58_Spr *unk124;
    u8 pad128[0x2CC - 0x128];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x327 - 0x325];
    u8 unk327;
    u8 pad328[0x32A - 0x328];
    s16 unk32A;
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x370 - 0x334];
    s16 unk370;
    u8 pad372[0x384 - 0x372];
    u8 unk384[0x390 - 0x384];
    u8 unk390[0x3DA - 0x390];
    s16 unk3DA;
};

typedef struct {
    u16 v;
    u8 pad2[0x62];
} UnkStruct_0234ac58_Tbl;

extern s16 data_0206e470[];
extern u32 gRandSeed;
extern UnkStruct_0234ac58_Tbl data_ov000_0236ab3a[];

#define IDX_0234ac58(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0234ac58(a) data_0206e470[(u32)(IDX_0234ac58(a) << 1)]
#define COS_0234ac58(a) data_0206e470[(u32)((IDX_0234ac58(a) << 1) + 1)]

void Player_SetAnimation(UnkStruct_0234ac58 *w, s32 state);
void Player_State_InAir(UnkStruct_0234ac58 *w);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void EffAirBubble_Spawn(UnkStruct_0234ac58 *w, s32 a, s32 b, u16 c);

static inline u16 Rand_0234ac58(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_Action_Jump(UnkStruct_0234ac58 *w) {
    Player_SetAnimation(w, 0x30);
    if (!(w->unk58 & 0x10)) {
        w->unk3DA = w->y >> 8;
    }
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk68 = Player_State_InAir;
    w->unk2E = (w->unk34 * COS_0234ac58((u16)(w->unk1 << 8))) >> 12;
    w->unk30 = (w->unk34 * SIN_0234ac58((u16)(w->unk1 << 8))) >> 12;
    w->unk2E += (w->unk370 * SIN_0234ac58((u16)(w->unk1 << 8)) << 4) >> 16;
    w->unk30 += (-w->unk370 * COS_0234ac58((u16)(w->unk1 << 8)) << 4) >> 16;
    if (w->unk330 & 0x1000) {
        w->unk32 = w->unk30;
        w->unk30 = 0;
        if (w->z < 0) {
            w->unk32 = -w->unk32;
        }
    }
    ObjRect_SetAtkDefFlags(w->unk2CC, 0x13, 0);
    w->unk32C &= ~0xf;
    w->unk32C &= ~0xc000;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
    if (w->unk324 == 1) {
        w->unk124->unk114 = 0x4000;
    }
    if (w->unk324 != 1) {
        Sound_PlaySeqArcEx(w->unk384, 0, 2);
    } else {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x13);
    }
    NNS_SndPlayerStopSeq(w->unk390, 0x20);
    NNS_SndHandleReleaseSeq(w->unk390);
    if (!(w->unk32C & 0x4000000)) {
        return;
    }
    if (!(Rand_0234ac58() & 1)) {
        return;
    }
    EffAirBubble_Spawn(w, 0, 0, data_ov000_0236ab3a[w->unk327].v);
}
