#include <ov000_c/stageobj.h>

typedef struct {
    u16 b0 : 5;
    u16 b5 : 11;
} UnkStruct_Bits5;

typedef struct {
    u8 pad0[0x24];
    UnkStruct_Bits5 unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x3C];
    s32 unk3C;
    s32 unk40;
} UnkStruct_0236a444;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x220 - 0x210];
    u32 unk220;
} UnkStruct_Boss;

typedef struct {
    u8 pad0[0x14C];
    s32 unk14C;
    s32 unk150;
    s32 unk154;
    u8 pad158[0x248 - 0x158];
    s32 unk248;
} UnkStruct_Fx;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x42C - 0x3D4];
    u16 unk42C;
    u8 pad42E[0x440 - 0x42E];
    void *unk440;
    u8 pad444[0x480 - 0x444];
    void *unk480;
} UnkStruct_022e8724;

extern volatile UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Bits5 data_02089250;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern UnkStruct_Boss *gBoss7;
extern s32 data_ov000_02363b74[];
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
UnkStruct_Fx *BossEffect_CreateHoming(s32 a, s32 x, s32 y, s32 z);
void Boss7PartD5_Unk_ov000_022eb3c4(UnkStruct_022e8724 *w);
void func_ov000_02326f20(void);
void Boss7PartD5_MoveState_Unk_ov000_022e86c0(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void Boss7PartD5_MoveState_Unk_ov000_022e8724(UnkStruct_022e8724 *w) {
    UnkStruct_Boss *b = gBoss7;
    UnkStruct_Fx *fx;

    if (gDisplayMain.unk24.b0 != 0) {
        data_02089250.b0 -= 1;
        data_ov000_0236a444.unk3C = ScreenShake_GetOffsetX() << 13;
        data_ov000_0236a444.unk40 = ScreenShake_GetOffsetY() << 13;
    }
    if (w->unk3D0 == 0) {
        if (!(b->unk220 & 7)) {
            fx = BossEffect_CreateHoming(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
            fx->unk248 = 0x2000;
            fx->unk14C = FX_Mul(fx->unk14C, 0xc00);
            fx->unk150 = FX_Mul(fx->unk150, 0xc00);
            fx->unk154 = FX_Mul(fx->unk154, 0xc00);
        }
    } else if (!(b->unk220 & 1)) {
        Boss7PartD5_Unk_ov000_022eb3c4(w);
    }
    w->base.unk2E = FX_Mul(COS_IDX((s32)((u16)w->base.unk8 & 0xffff) >> 4), data_ov000_02363b74[w->unk42C]) >> 4;
    w->base.unk30 = FX_Mul(SIN_IDX((s32)((u16)w->base.unk8 & 0xffff) >> 4), data_ov000_02363b74[w->unk42C]) >> 4;
    if (!(w->base.unk58 & 1))
        return;
    if (gDisplayMain.unk24.b0 != 0)
        return;
    w->base.unk30 = 0;
    w->base.unk58 |= 0x80;
    if (w->unk480 != NULL) {
        func_ov000_02326f20();
        w->unk480 = NULL;
    }
    b->unk20C &= ~0x10;
    w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e86c0;
}
