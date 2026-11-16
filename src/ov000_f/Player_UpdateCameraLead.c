#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    s16 unk2A;
} UnkGround_023458cc;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x80 - 0x2C];
    UnkGround_023458cc *unk80;
    u8 pad84[0x325 - 0x84];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u8 pad330[0x3D1 - 0x330];
    s8 unk3D1;
    s16 unk3D2;
    s16 unk3D4;
    s16 unk3D6;
    s16 unk3D8;
} UnkStruct_023458cc;

#define ABS(x) ((x) < 0 ? -(x) : (x))

BOOL Display_IsMainScreenOnBottom(void);
void func_ov000_0233b2c8(s32 x, s32 y);

void Player_UpdateCameraLead(UnkStruct_023458cc *w) {
    s32 shift = 4;
    s32 tx;
    s32 ty;
    s32 cx;
    s32 cy;
    s32 gx;
    s32 gy;

    if (w->unk325 != 0) {
        return;
    }
    if (Display_IsMainScreenOnBottom()) {
        cx = 0x80;
        cy = 0x58;
    } else {
        cx = 0x80;
        cy = 0x60;
    }
    if (w->unk3D1 != 0) {
        w->unk3D1--;
        tx = w->unk3D6 + (w->unk28 << 4);
        ty = w->unk3D8 + (w->unk2A << 4);
        shift = 2;
    } else {
        gx = 0;
        gy = 0;
        if (w->unk80 != NULL) {
            gx = w->unk80->unk28;
            gy = w->unk80->unk2A;
        }
        tx = (w->unk1C - (w->x - gx)) << 4;
        ty = (w->unk20 - (w->y - gy)) << 3;
        if (ABS(tx) < 0x1000) {
            tx = 0;
        }
        if (ABS(ty) < 0x2800) {
            ty = 0;
        }
    }
    if ((w->unk32C & 0x80) || w->unk3D1 != 0) {
        if (tx > 0x5800) {
            tx = 0x5800;
        } else if (tx < -0x5800) {
            tx = -0x5800;
        }
        if (ty > 0x1600) {
            ty = 0x1600;
        } else if (ty < -0x4800) {
            ty = -0x4800;
        }
    } else {
        if (tx > 0x2c00) {
            tx = 0x2c00;
        } else if (tx < -0x2c00) {
            tx = -0x2c00;
        }
        if (ty > 0xb00) {
            ty = 0xb00;
        } else if (ty < -0x2400) {
            ty = -0x2400;
        }
    }
    w->unk3D6 = tx;
    w->unk3D8 = ty;
    if (w->unk32C & 0x2000) {
        w->unk3D2 -= w->unk3D2 >> 2;
        w->unk3D4 -= w->unk3D4 >> 2;
    } else {
        w->unk3D2 += (w->unk3D6 - w->unk3D2) >> shift;
        w->unk3D4 += (w->unk3D8 - w->unk3D4) >> shift;
    }
    func_ov000_0233b2c8(w->unk3D2 + (cx << 8), w->unk3D4 + (cy << 8));
}
