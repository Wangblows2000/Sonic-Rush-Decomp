#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkState_02059a50;

extern UnkState_02059a50 gGameState;
extern u16 data_022c4752[][3][5];
extern u16 data_022c4828[][2][5];

s32 func_02059a50(s32 zone, s32 act, u32 score) {
    BOOL b = FALSE;
    u16 *p;
    s32 i;

    if (score > 35999)
        score = 35999;
    if (gGameState.zone >= 7 && gGameState.zone < 9 && gGameState.act < 2)
        b = TRUE;
    if (b)
        p = data_022c4828[zone - 7][act];
    else
        p = data_022c4752[zone][act];
    for (i = 0; i < 5; i++) {
        if (score < *p)
            return i;
        p++;
    }
    return i;
}
