#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_02351300_Obj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 pad4[5 - 4];
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x30 - 0x2C];
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x3E - 0x36];
    s16 unk3E;
    u8 pad40[0x5C - 0x40];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_02351300_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
} UnkStruct_02351300;

s16 ObjSpd_UpSet(s32 v, s32 a, s32 max);
void FixInformation_Unk_ov000_0232f81c(UnkStruct_02351300 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Math_RotateVec2S16(s16 *vx, s16 *vy, u8 a);

void Player_Unk_ov000_02351300(UnkStruct_02351300 *w) {
    s16 vx;
    s16 vy;

    w->unk3F4 -= w->unk34;
    if (w->unkC > 0) {
        w->unk34 = ObjSpd_UpSet(w->unk34, -0x18, w->unk3E);
        if (w->unk34 < 0x300 && !(w->unk32C & 2)) {
            w->unk32C |= 2;
            w->unk30 = -0x300;
            FixInformation_Unk_ov000_0232f81c(w);
        }
        if ((w->unk32C & 4) && w->unk3F4 > w->unk3F0) {
            w->unk3F4 = w->unk3F0;
        }
    } else {
        w->unk34 = ObjSpd_UpSet(w->unk34, 0x18, w->unk3E);
        if (w->unk34 > -0x300 && !(w->unk32C & 2)) {
            w->unk32C |= 2;
            w->unk30 = -0x300;
            FixInformation_Unk_ov000_0232f81c(w);
        }
        if ((w->unk32C & 4) && w->unk3F4 < w->unk3F0) {
            w->unk3F4 = w->unk3F0;
        }
    }
    w->unk1 = (w->unk3F4 & 0xffff) >> 8;
    w->unk3F8 -= w->unk30;
    if ((w->unk32C & 2) && !(w->unk32C & 4)) {
        w->unk3 += 8;
        if (w->unk3 > 0x80) {
            w->unk3 = 0x80;
        }
        w->unk30 = ObjSpd_UpSet(w->unk30, 0x2a, w->unk3E);
        if (w->unk3F8 < 0) {
            w->unk3F8 = 0;
            w->unk30 = 0;
            w->unk3 = 0;
            w->unk5C ^= 1;
            w->unk32C |= 4;
            Sound_PlaySeqArcEx(w->unk384, 0, 0x62);
        }
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    vx = w->unk3F8 >> 8;
    if (w->unkC <= 0) {
        vx = -vx;
    }
    vy = w->unk3FC >> 8;
    Math_RotateVec2S16(&vx, &vy, w->unk1 + w->unk5);
    w->x = w->unk3E8->x + (vx << 8);
    w->y = w->unk3E8->y + (vy << 8);
    w->unk28 = w->x - w->unk1C;
    w->unk2A = w->y - w->unk20;
}
