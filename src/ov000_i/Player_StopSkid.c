#include <nitro/types.h>

void NNS_SndPlayerStopSeq(void *p, s32 a);

void Player_StopSkid(u8 *w) {
    if (*(s16 *)(w + 0x328) >= 5 && *(s16 *)(w + 0x328) <= 10)
        NNS_SndPlayerStopSeq(w + 0x384, 0);
    if (*(s16 *)(w + 0x328) == 0 || (*(s16 *)(w + 0x328) >= 5 && *(s16 *)(w + 0x328) <= 10)) {
        if (*(u32 *)(w + 0x32c) & 1)
            *(u16 *)(w + 0x5c) |= 1;
        else
            *(u16 *)(w + 0x5c) &= ~1;
    }
}
