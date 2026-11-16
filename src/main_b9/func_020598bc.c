#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern u8 data_022c4824[][2];
extern u16 data_022c4828[][2][5];
extern u16 data_022c4830[][2][5];
extern u8 data_022c473c[][3];
extern u16 data_022c4752[][3][5];
extern u16 data_022c475a[][3][5];

s32 func_02059a50(s32 stage, s32 act, u32 time);

void func_020598bc(s32 stage, s32 act, u32 time, u32 flag) {
    s32 bit;
    s32 n;
    s32 rank;
    u8 *flags;
    u16 *p;
    u16 *times;
    BOOL special;

    if (time > 35999)
        time = 35999;
    rank = func_02059a50(stage, act, time);
    if (rank >= 5)
        return;
    n = 4 - rank;
    special = FALSE;
    if (gGameState.unk1c >= 7 && gGameState.unk1c < 9 && gGameState.unk1d < 2)
        special = TRUE;
    if (special) {
        s32 k = stage - 7;
        flags = &data_022c4824[k][act];
        p = data_022c4830[k][act];
        times = data_022c4828[k][act];
    } else {
        flags = &data_022c473c[stage][act];
        p = data_022c475a[stage][act];
        times = data_022c4752[stage][act];
    }
    bit = 4;
    for (; n > 0; n--) {
        *flags &= ~(1 << bit);
        *flags |= (*flags & (1 << (bit - 1))) << 1;
        bit--;
        *p = p[-1];
        p--;
    }
    times[rank] = time;
    *flags &= ~(1 << rank);
    *flags |= (u8)(flag & 1) << rank;
}
