#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022ece30;

typedef struct {
    u8 pad0[0x2c];
    s32 unk2c;
    s32 unk30;
    s32 unk34;
} UnkStruct_022ece30;

extern UnkTouch_022ece30 data_022b65b4;

#pragma thumb on
BOOL func_ov001_022ece30(UnkStruct_022ece30 *p) {
    s32 x;
    s32 y;
    s32 top;
    s32 h;

    if (p->unk2c == 0) {
        return FALSE;
    }
    if (data_022b65b4.flags & 0x10) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        if (x < 0xd8 || x >= 0xee) {
            return FALSE;
        }
        top = p->unk30 / 2 + 0x25;
        h = 0x57 - p->unk34 / 2;
        if (y >= top && y < top + h) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma thumb reset
