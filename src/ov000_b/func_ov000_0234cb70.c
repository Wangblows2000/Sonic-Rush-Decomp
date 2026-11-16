#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u16 unk0;
    u8 pad2[0x62];
} UnkStruct_0236ab3a;

typedef struct {
    u8 pad0[0x12C];
    s16 unk12C;
} UnkStruct_0234cb70_Sub;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x14 - 4];
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x64 - 0x5E];
    s8 unk64;
    s8 unk65;
    s8 unk66;
    s8 unk67;
    u8 pad68[0x6C - 0x68];
    void *unk6C;
    u8 pad70[4];
    void *unk74;
    void *unk78;
    u8 pad7C[4];
    s32 unk80;
    u8 pad84[0x124 - 0x84];
    UnkStruct_0234cb70_Sub *unk124;
    u8 pad128[0x2A0 - 0x128];
    UnkStruct_Coll coll2A0;
    UnkStruct_Coll coll2CC;
    UnkStruct_Coll coll2F8;
    u8 unk324;
    u8 unk325;
    u8 unk326;
    u8 unk327;
    u8 pad328[4];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x372 - 0x334];
    s16 unk372;
    u8 pad374[0x388 - 0x374];
    u8 unk388[8];
    u8 unk390[0x3B2 - 0x390];
    s16 unk3B2;
    u8 pad3B4[0x3C4 - 0x3B4];
    s16 unk3C4;
    u8 pad3C6[0x3CD - 0x3C6];
    u8 unk3CD;
    u8 unk3CE;
    u8 unk3CF;
    u8 pad3D0[0x3DA - 0x3D0];
    s16 unk3DA;
    u8 pad3DC[0x3E0 - 0x3DC];
    s16 unk3E0;
    u8 pad3E2[0x3E8 - 0x3E2];
    s32 unk3E8;
    s16 unk3EC;
    s16 unk3EE;
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
    u8 pad400[0x410 - 0x400];
    u8 unk410;
} UnkStruct_0234cb70;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0236ab3a data_ov000_0236ab3a[];

void Player_In(void);
void Player_SpriteCallback(void);
void Player_Collide(void);
void Player_OnHurt(void);
void func_ov000_02342b78(void);
void func_ov000_0234c644(UnkStruct_0234cb70 *w);
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);

void func_ov000_0234cb70(UnkStruct_0234cb70 *w) {
    if (data_022c4560.unk1C == 1) {
        if (data_ov000_0236ab3a[w->unk327].unk0 != 0) {
            if ((w->unk14 >> 8) > data_ov000_0236ab3a[w->unk327].unk0)
                w->unk32C |= 0x4000000;
            else
                w->unk32C &= ~0x4000000;
        } else {
            w->unk32C &= ~0x4000000;
        }
    }
    w->unk6C = Player_In;
    w->unk74 = Player_SpriteCallback;
    w->unk78 = Player_Collide;
    w->unk64 = -7;
    w->unk66 = 7;
    w->unk65 = -0xc;
    w->unk67 = 0xd;
    w->coll2A0.unk1C = Player_OnHurt;
    w->coll2A0.unk28 = 0x21;
    w->coll2A0.unkA &= ~0x300;
    w->coll2A0.unkA |= 0x20;
    if (w->unk3B2 == 0)
        ObjRect_SetAtkDefFlags(&w->coll2A0, 0xa, 2);
    w->coll2CC.unk28 = 0x21;
    w->coll2CC.unk1C = NULL;
    w->coll2CC.unkA &= ~8;
    w->coll2CC.unkA &= ~0x300;
    w->coll2CC.unkA |= 0x20;
    ObjRect_SetAtkDefFlags(&w->coll2CC, 0, 0);
    if ((data_022c4560.unk18 & 0x20) && w->unk325 != 0) {
        w->coll2F8.unk1C = func_ov000_02342b78;
        w->coll2F8.unk28 = 2;
        w->coll2F8.unkA |= 0xc0;
        w->coll2F8.unkA |= 0x20;
        w->coll2F8.unk18 = w;
        ObjRect_SetAtkDefFlags(&w->coll2F8, 0, -0x10);
        ObjRect_SetBox2D(&w->coll2F8, -0x30, -0x20, 0x30, 0x20);
    }
    func_ov000_0234c644(w);
    w->unk1 = 0;
    w->unk2 = 0;
    w->unk3 = 0;
    w->unk34 = 0;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk32 = 0;
    w->unk3C4 = 0;
    if (!(w->unk330 & 0x100))
        w->unk18 = 0;
    w->unk80 = 0;
    if (!(w->unk330 & 0x600)) {
        w->unk3E8 = 0;
        w->unk3EC = 0;
        w->unk3EE = 0;
        w->unk330 &= ~0x38;
        w->unk330 &= ~0xc0;
    }
    w->unk3F0 = 0;
    w->unk3F4 = 0;
    w->unk3F8 = 0;
    w->unk3FC = 0;
    w->unk3CD = 0;
    w->unk3CE = 0;
    w->unk3CF = 0;
    w->unk3E0 = 0x78;
    w->unk372 = 0;
    w->unk3DA = 0;
    if (w->unk410 & 0x80)
        w->unk54 |= 1;
    w->unk330 &= ~0x20003;
    w->unk330 &= ~0x40000;
    w->unk330 &= ~0x400000;
    w->unk32C &= ~0x80000;
    w->unk32C &= ~0xf;
    w->unk32C &= ~0xc040;
    w->unk32C &= ~0x102000;
    w->unk32C &= ~0x400000;
    w->unk32C &= ~0x400;
    w->unk54 &= ~2;
    w->unk54 |= 0x10;
    w->unk5C &= ~0x20;
    if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
        w->unk5C |= 0x200;
    w->unk5C |= 0x100;
    w->unk58 |= 0x80;
    w->unk58 |= 0x40;
    w->unk58 |= 0x20000;
    w->unk58 |= 0x40000;
    w->unk58 |= 0x80000;
    w->unk58 |= 0x200000;
    w->unk58 &= ~0xf;
    w->unk58 &= ~0x100;
    w->unk58 &= ~0x200;
    w->unk58 &= ~0x10;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x8000;
    w->unk58 &= ~0x4000;
    w->unk58 &= ~0x10000000;
    if (w->unk124 != NULL)
        w->unk124->unk12C = 0x200;
    NNS_SndPlayerStopSeq(w->unk388, 0);
    NNS_SndPlayerStopSeq(w->unk390, 0x20);
    NNS_SndHandleReleaseSeq(w->unk390);
}
