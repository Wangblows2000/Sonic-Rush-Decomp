#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[2];
    u16 flags;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} UnkBox_0206825c;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_0206825c;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x68 - 0xc];
    s32 unk68;
    s32 unk6C;
} UnkCam_0206825c;

typedef struct {
    u16 x;
    u16 y;
} UnkPos_0206825c;

extern UnkCam_0206825c data_ov000_0236aad8;
extern UnkPos_0206825c gTouchState;

BOOL ObjRect_PointCheck(UnkBox_0206825c *b, s32 px, s32 py);

BOOL func_0206825c(UnkObj_0206825c *o, UnkBox_0206825c *b) {
    s32 cx;
    s32 cy;

    b->x = (s16)(o->x >> 8);
    b->y = (s16)(o->y >> 8);
    if (data_ov000_0236aad8.unk8 > data_ov000_0236aad8.unk6C) {
        cx = (s16)(data_ov000_0236aad8.unk4 >> 8);
        cy = (s16)(data_ov000_0236aad8.unk8 >> 8);
    } else {
        cx = (s16)(data_ov000_0236aad8.unk68 >> 8);
        cy = (s16)(data_ov000_0236aad8.unk6C >> 8);
    }
    return ObjRect_PointCheck(b, gTouchState.x + cx, gTouchState.y + cy);
}
