#include <nitro/types.h>

typedef struct {
    s16 unk0;
    u8 pad2[0x30 - 2];
} UnkStruct_02368ba0;

extern UnkStruct_02368ba0 data_ov000_02368ba0[];
s16 ObjSpd_DownSet(s32 a, s32 b);

void Player_ApplyRollFriction(u8 *w) {
    s32 v;

    if (*(s16 *)(w + 0x352) != 0) {
        (*(s16 *)(w + 0x352))--;
        *(s16 *)(w + 0x3c) = 0;
    } else {
        *(s16 *)(w + 0x3c) = data_ov000_02368ba0[*(u8 *)(w + 0x324)].unk0;
        *(u8 *)(w + 4) = 0xe;
    }
    v = *(s16 *)(w + 0x34);
    if ((v > 0 && (*(u16 *)(w + 0x420) & 0x10)) || (v < 0 && (*(u16 *)(w + 0x420) & 0x20))) {
        *(s16 *)(w + 0x34) = ObjSpd_DownSet(v, *(s16 *)(w + 0x362) >> 1);
        return;
    }
    if ((v <= 0 || !(*(u16 *)(w + 0x420) & 0x10)) && (v >= 0 || !(*(u16 *)(w + 0x420) & 0x20))) {
        *(s16 *)(w + 0x34) = ObjSpd_DownSet(v, *(s16 *)(w + 0x362) << 1);
        return;
    }
    *(s16 *)(w + 0x34) = ObjSpd_DownSet(v, *(s16 *)(w + 0x362));
}
