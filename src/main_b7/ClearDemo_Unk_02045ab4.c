#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 time;
} UnkStruct_022c4590;

typedef struct {
    u8 pad[0x1c];
    u8 zone;
    u8 act;
} UnkStruct_022c4560;

extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_022c4560 gGameState;
extern u32 *data_02079198[][2];
extern u32 data_02078ef0[];

#pragma thumb on
u32 ClearDemo_Unk_02045ab4(void) {
    u32 r;
    s32 i;
    u32 *p;
    u32 t;
    t = data_022c4590.time;
    p = data_02079198[gGameState.zone][gGameState.act];
    for (i = 0, r = 0; i < 10; i++, p++) {
        if (*p <= t) {
            r = data_02078ef0[i];
            break;
        }
    }
    return r;
}
#pragma thumb reset
