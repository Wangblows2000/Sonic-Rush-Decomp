#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_02345aa8_Obj;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x325 - 0x18];
    u8 unk325;
    u8 pad326;
    u8 unk327;
    u8 pad328[0x330 - 0x328];
    u32 unk330;
    u8 pad334[0x3D2 - 0x334];
    s16 unk3D2;
    s16 unk3D4;
    u8 pad3D6[0x3E8 - 0x3D6];
    UnkStruct_02345aa8_Obj *unk3E8;
    s16 unk3EC;
    s16 unk3EE;
} UnkStruct_02345aa8;

typedef struct {
    s32 v;
    u8 pad4[0x60];
} UnkStruct_02345aa8_L;

extern UnkStruct_02345aa8_L gCamera[];
extern UnkStruct_02345aa8_L data_ov000_0236aadc[];
extern UnkStruct_02345aa8_L data_ov000_0236aae0[];
extern u32 gObjManagerFlags;

#define ABS_02345aa8(x) ((x) < 0 ? -(x) : (x))

s32 ObjShift_Set(s32 cur, s32 target, s32 a, s32 b, s32 step);
void Camera_LockX(u8 idx);
void Camera_UnlockX(u8 idx);
void Camera_LockY(u8 idx);
void Camera_UnlockY(u8 idx);
void func_ov000_0233e434(s32 v, u8 idx);
void func_ov000_0233e39c(s32 v, u8 idx);

void Player_UpdateCamera(UnkStruct_02345aa8 *w) {
    s32 t;
    s32 cur;
    s32 d;

    if (w->unk325 != 0) {
        return;
    }
    if (w->unk330 & 0x10) {
        if (!(gCamera[w->unk327].v & 0x40)) {
            Camera_LockX(w->unk327);
        }
        if (w->unk3E8 != NULL) {
            t = w->unk3E8->x - ((w->unk3EC + 0x80) << 8);
            if (w->unk330 & 0x40) {
                data_ov000_0236aadc[w->unk327].v = t;
                func_ov000_0233e434(t, w->unk327);
            } else {
                if (gObjManagerFlags & 0x100) {
                    data_ov000_0236aadc[w->unk327].v = ObjShift_Set(data_ov000_0236aadc[w->unk327].v, t, 4, 0, 0x200);
                } else {
                    data_ov000_0236aadc[w->unk327].v = ObjShift_Set(data_ov000_0236aadc[w->unk327].v, t, 4, 0, 0x100);
                }
                if ((t & ~0x1ff) == (data_ov000_0236aadc[w->unk327].v & ~0x1ff)) {
                    w->unk330 |= 0x40;
                }
            }
        } else {
            w->unk330 &= ~0x50;
        }
    } else {
        w->unk330 &= ~0x40;
        if (gCamera[w->unk327].v & 0xc0) {
            if (ABS_02345aa8(data_ov000_0236aadc[w->unk327].v - (w->x - 0x8000)) > 0x80) {
                data_ov000_0236aadc[w->unk327].v += ((w->x - 0x8000) - data_ov000_0236aadc[w->unk327].v) >> 2;
            } else {
                data_ov000_0236aadc[w->unk327].v = w->x - 0x8000;
            }
            d = ABS_02345aa8(data_ov000_0236aadc[w->unk327].v - (w->x - 0x8000));
            if (d <= 0x400) {
                Camera_UnlockX(w->unk327);
                w->unk3D2 = 0;
            }
        }
    }
    if (w->unk330 & 0x20) {
        if (!(gCamera[w->unk327].v & 0x80)) {
            Camera_LockY(w->unk327);
        }
        if (w->unk3E8 != NULL) {
            t = w->unk3E8->y - ((w->unk3EE + 0x60) << 8);
            if (w->unk330 & 0x80) {
                data_ov000_0236aae0[w->unk327].v = t;
                func_ov000_0233e39c(t, w->unk327);
                return;
            }
            data_ov000_0236aae0[w->unk327].v = ObjShift_Set(data_ov000_0236aae0[w->unk327].v, t, 4, 0, 0x100);
            if ((t & ~0x1ff) == (data_ov000_0236aae0[w->unk327].v & ~0x1ff)) {
                w->unk330 |= 0x80;
            }
        } else {
            w->unk330 &= ~0xa0;
        }
        return;
    }
    w->unk330 &= ~0x80;
    if (!(gCamera[w->unk327].v & 0x80)) {
        return;
    }
    if (ABS_02345aa8(data_ov000_0236aae0[w->unk327].v - (w->y - 0x6000)) > 0x80) {
        data_ov000_0236aae0[w->unk327].v += ((w->y - 0x6000) - data_ov000_0236aae0[w->unk327].v) >> 2;
    } else {
        data_ov000_0236aae0[w->unk327].v = w->y - 0x6000;
    }
    d = ABS_02345aa8(data_ov000_0236aae0[w->unk327].v - (w->y - 0x6000));
    if (d > 0x400) {
        return;
    }
    Camera_UnlockY(w->unk327);
    w->unk3D4 = 0;
}
