#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_0235020c_Obj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[5 - 2];
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x5C - 0x32];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x3E8 - 0x334];
    UnkStruct_0235020c_Obj *unk3E8;
    u8 pad3EC[0x3EE - 0x3EC];
    s16 unk3EE;
    s32 unk3F0;
} UnkStruct_0235020c;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_0235020c_4560;

extern UnkStruct_0235020c_4560 gGameState;
extern u32 gRandSeed;

s32 ObjShift_SetFromRef(s32 cur, s32 target, s32 c, s32 d, s32 e, s32 f);
u8 ObjAngle_MoveToward8(u8 cur, s32 target, s32 step);
void func_ov000_02330098(UnkStruct_0235020c *w, s16 a, s16 b);
void Player_StopHighSpeed(UnkStruct_0235020c *w);
void Player_StopBoost(UnkStruct_0235020c *w);
void BrightnessFade_Create(s32 a, s32 b);

static inline u16 Rand_0235020c(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_State_GmkSpecialEnter(UnkStruct_0235020c *w) {
    s32 t = 0x50;
    s32 py;
    s32 p2;

    w->unk2E = ObjShift_SetFromRef(w->x, w->unk3E8->x, w->unk3F0, 1, 0xf00, 0x400) - w->x;
    w->unk1 = ObjAngle_MoveToward8(w->unk1, 0xc0, 0x10);
    if (w->unk5 != 0) {
        t = -0x50;
        w->unk2E = -w->unk2E;
    }
    w->unk330 &= ~0xc0;
    w->unk3EE = ObjShift_SetFromRef(w->unk3EE, t, 0, 4, 0xf00, 0x80);
    if (!(gGameState.unk20 & 1)) {
        func_ov000_02330098(w, 0xf - (Rand_0235020c() & 0x1e), 0xf - (Rand_0235020c() & 0x1e));
    }
    py = w->unk3E8->y;
    if (((w->y < py - 0x3000 && w->unk5 == 0) || (w->y > py + 0x3000 && w->unk5 != 0)) && !(w->unk5C & 0x20)) {
        Player_StopHighSpeed(w);
        Player_StopBoost(w);
        w->unk5C |= 0x20;
        w->unk32C |= 0x200000;
    }
    p2 = w->unk3E8->y;
    if ((w->y < p2 - 0xe800 && w->unk5 == 0) || (w->y > p2 + 0xe800 && w->unk5 != 0)) {
        w->unk30 = 0;
    }
    if (w->unkC == 0) {
        w->unk32C |= 0x40000000;
        return;
    }
    w->unkC--;
    if (w->unkC == 0x10) {
        BrightnessFade_Create(5, 0x100);
    }
}
