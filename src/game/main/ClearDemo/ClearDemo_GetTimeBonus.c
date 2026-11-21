#include <nitro/types.h>

typedef struct {
    u8 pad[0x24];
    u32 flags;
} UnkStruct_022c4590;

typedef struct {
    u8 pad[0x1c];
    u8 zone;
    u8 act;
    u8 pad1e[2];
    u32 score;
} UnkStruct_022c4560;

extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_022c4560 gGameState;
extern u32 *data_020791d0[][3];
extern u32 data_02079008[];
extern u32 data_02078f90[];

#pragma thumb on
u32 ClearDemo_GetTimeBonus(void) {
    u32 *p;
    u32 score;
    u8 zone;
    u8 act;
    s32 i;
    u32 r;
    act = gGameState.act;
    zone = gGameState.zone;
    p = data_020791d0[zone][act];
    score = gGameState.score;
    if (data_022c4590.flags & 4)
        return 0;
    for (i = 0, r = 0; i < 10; i++, p++) {
        if (*p >= score) {
            if (zone >= 9 || act >= 2)
                r = data_02079008[i];
            else
                r = data_02078f90[i];
            break;
        }
    }
    return r;
}
#pragma thumb reset
