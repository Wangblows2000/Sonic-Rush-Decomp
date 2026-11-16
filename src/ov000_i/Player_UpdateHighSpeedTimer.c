#include <nitro/types.h>

typedef struct {
    s16 unk0;
    u8 pad2[0x30 - 2];
} UnkStruct_02368ba8;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02347b4c;

extern UnkCfg_02347b4c gGameState;
extern UnkStruct_02368ba8 data_ov000_02368ba8[];
void Player_StartHighSpeed(u8 *w);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Player_UpdateHighSpeedTimer(u8 *w) {
    s16 lim = data_ov000_02368ba8[*(u8 *)(w + 0x324)].unk0;
    s32 v;

    if (gGameState.unk1C >= 9)
        return;
    if (gGameState.unk1D >= 2)
        return;
    if (*(u32 *)(w + 0x32c) & 0x80) {
        *(s16 *)(w + 0x3ac) = lim;
        return;
    }
    if (!(*(u16 *)(w + 0x420) & 0x30))
        return;
    v = *(s16 *)(w + 0x34);
    if (ABS(v) < *(s16 *)(w + 0x358))
        return;
    (*(s16 *)(w + 0x3ac))++;
    if (*(s16 *)(w + 0x3ac) < lim)
        return;
    if (*(u32 *)(w + 0x58) & 1)
        Player_StartHighSpeed(w);
    *(s16 *)(w + 0x3ac) = lim;
}
