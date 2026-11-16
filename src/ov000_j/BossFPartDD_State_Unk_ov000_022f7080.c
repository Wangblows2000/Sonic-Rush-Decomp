#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10d];
    u8 unk10D;
    u8 unk10E;
} UnkStruct_022f7080_cam;

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x54 - 0x10];
    u32 unk54;
    u8 pad58[0x5c - 0x58];
    u16 unk5C;
    u8 pad5E[0x16a - 0x5e];
    u16 unk16A;
    u8 pad16C[0x218 - 0x16c];
    UnkStruct_022f7080_cam cam;
} UnkStruct_022f7080;

void BossFPartDD_State_Unk_ov000_022f7080(UnkStruct_022f7080 *p) {
    UnkStruct_022f7080_cam *cam = &p->cam;

    p->unkC++;
    if (p->unkC > 8) {
        p->unk16A &= ~4;
    } else if (p->unkC >= 0) {
        p->unk5C &= ~0x20;
        p->unk16A |= 4;
    }
    if (!(cam->unk10D & 2))
        return;
    if (cam->unk10E & 2)
        p->unk54 |= 4;
}
