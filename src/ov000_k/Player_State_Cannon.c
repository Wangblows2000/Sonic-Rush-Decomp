#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 x;
    s32 y;
} UnkStruct_02350d3c_Obj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 pad4[5 - 4];
    u8 unk5;
    u8 pad6[8 - 6];
    u32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x58 - 0x2C];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[4];
    u8 unk388[0x3D1 - 0x388];
    u8 unk3D1;
    u8 pad3D2[0x3E8 - 0x3D2];
    UnkStruct_02350d3c_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    u8 pad3F4[0x3F8 - 0x3F4];
    s32 unk3F8;
    u8 pad3FC[0x422 - 0x3FC];
    u16 unk422;
} UnkStruct_02350d3c;

extern s16 data_0206e470[];

#define IDX_02350d3c(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02350d3c(a) data_0206e470[(u32)(IDX_02350d3c(a) << 1)]
#define COS_02350d3c(a) data_0206e470[(u32)((IDX_02350d3c(a) << 1) + 1)]

s32 ObjSpd_UpSet(s32 v, s32 a, s32 max);
void func_0201c070(void *p, s32 a);
void Math_RotateVec2S16(s16 *vx, s16 *vy, u8 a);
void func_ov000_0232e48c(UnkStruct_02350d3c *w, s32 vx, s32 vy);
void func_ov000_02346d30(UnkStruct_02350d3c *w);
void func_ov000_023558bc(UnkStruct_02350d3c *w, s32 vx, s32 vy);
void Player_SetAnimation(UnkStruct_02350d3c *w, s32 state);
void ScreenShake_Start(s32 a);
void ScreenFlash_Create(s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_State_Cannon(UnkStruct_02350d3c *w) {
    s16 vx;
    s16 vy;
    s32 sx;
    s32 sy;
    s16 t;

    if (w->unk3E8 != NULL) {
        w->unk3F0 = (s16)ObjSpd_UpSet(w->unk3F0, 0x10, 0x400);
        w->unkC -= w->unk3F0 >> 2;
        if (w->unkC < 0) {
            w->unkC = 0;
            w->unk8++;
            w->unk32C &= ~0x2000;
            w->unk330 &= ~0x30;
        }
        w->unk3F8 -= w->unk3F0 << 2;
        w->unk1 = (w->unk3F8 & 0xffff) >> 8;
        w->unk1C = w->x;
        w->unk20 = w->y;
        w->x = w->unk3E8->x - ((w->unkC * COS_02350d3c((u16)(w->unk1 << 8))) >> 12);
        w->y = w->unk3E8->y - ((w->unkC * SIN_02350d3c((u16)(w->unk1 << 8))) >> 12);
        w->unk28 = w->x - w->unk1C;
        w->unk2A = w->y - w->unk20;
    }
    if (w->unk3E8 != NULL) {
        if (!(w->unk422 & 3) && w->unk8 <= 0x258) {
            return;
        }
        if (w->unkC != 0) {
            return;
        }
    }
    vx = 0x38;
    vy = 0;
    func_0201c070(w->unk388, 0);
    sx = 0xa00;
    if (w->unk3E8 != NULL) {
        w->unk1 = w->unk3E8->unk1;
        t = sx;
        sx = (s16)((COS_02350d3c((u16)(w->unk1 << 8)) * t) >> 12);
        sy = (s16)((SIN_02350d3c((u16)(w->unk1 << 8)) * t) >> 12);
        Math_RotateVec2S16(&vx, &vy, w->unk1 + w->unk5);
        func_ov000_0232e48c(w, vx, vy);
    }
    w->unk3 = 0;
    w->unk3E8 = NULL;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x100;
    w->unk32C &= ~0x2000;
    w->unk32C &= ~0x100000;
    func_ov000_02346d30(w);
    func_ov000_023558bc(w, sx, sy);
    Player_SetAnimation(w, 0x30);
    w->unk5C |= 4;
    w->unkC = 0x1e;
    w->unk58 &= ~0x80;
    ScreenShake_Start(4);
    ScreenFlash_Create(0);
    w->unk3D1 = 8;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x61);
}
