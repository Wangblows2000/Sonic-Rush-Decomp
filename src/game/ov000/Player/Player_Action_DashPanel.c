#include <nitro/types.h>

typedef struct UnkStruct_023495a8 UnkStruct_023495a8;

struct UnkStruct_023495a8 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x352 - 0x5E];
    s16 unk352;
    u8 pad354[0x384 - 0x354];
    u8 unk384[4];
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Player_StartHighSpeed(UnkStruct_023495a8 *w);

void Player_Action_DashPanel(UnkStruct_023495a8 *w, s16 vx, s16 vy) {
    u32 dir;
    u32 d3;

    w->unk352 = 0x80;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x40);
    if (vx < 0) {
        w->unk5C |= 1;
    } else {
        w->unk5C &= ~1;
    }
    if (w->unk58 & 0x10) {
        dir = w->unk5C & 1;
        if ((dir && w->unk2E > vx) || (!dir && w->unk2E < vx)) {
            w->unk2E = vx;
        }
        if (ABS(w->unk30) < ABS(vy)) {
            w->unk30 = vy;
        }
    } else {
        switch (((w->unk1 + 0x20) & 0xc0) >> 6) {
        case 0:
        case 2:
            dir = w->unk5C & 1;
            if ((dir && w->unk34 > vx) || (!dir && w->unk34 < vx)) {
                w->unk34 = vx;
            }
            if (ABS(w->unk30) < ABS(vy)) {
                w->unk30 = vy;
                if (w->unk30 < 0) {
                    w->unk58 |= 0x10;
                }
            }
            break;
        case 1:
            dir = w->unk5C & 1;
            if ((dir && w->unk34 > vy) || (!dir && w->unk34 < vy)) {
                w->unk34 = vy;
            }
            if (ABS(w->unk2E) < ABS(vx)) {
                w->unk2E = vx;
            }
            break;
        case 3:
            d3 = w->unk5C & 1;
            if ((d3 && w->unk34 > -vy) || (!d3 && w->unk34 < -vy)) {
                w->unk34 = -vy;
            }
            if (ABS(w->unk2E) < ABS(vx)) {
                w->unk2E = vx;
            }
            break;
        }
    }
    Player_StartHighSpeed(w);
}
