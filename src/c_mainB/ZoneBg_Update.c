#include <nitro/types.h>

typedef struct UnkDisp_02341ebc {
    u8 pad[4];
    u16 x;
    u16 y;
} UnkDisp_02341ebc;

typedef struct UnkCam_02341ebc {
    u8 pad[0x2c];
    s32 x;
    s32 y;
    u8 pad2[0x64 - 0x34];
} UnkCam_02341ebc;

typedef struct UnkState_02341ebc {
    u8 pad[0x1c];
    u8 zone;
    u8 act;
} UnkState_02341ebc;

typedef void (*ZoneBgFunc_02341ebc)(void *arg);

extern u16 data_ov000_0236aab0;
extern UnkState_02341ebc gGameState;
extern ZoneBgFunc_02341ebc data_ov000_02368b08[][3];
extern void *data_ov000_0236ac1c;
extern UnkCam_02341ebc gCamera[];
extern UnkDisp_02341ebc *gDisplayTable[];

void ZoneBg_Update(void)
{
    UnkCam_02341ebc *cam;
    ZoneBgFunc_02341ebc func;
    u8 i;

    if (!(data_ov000_0236aab0 & 1))
        return;
    func = data_ov000_02368b08[gGameState.zone][gGameState.act];
    if (func != NULL)
        func(data_ov000_0236ac1c);
    cam = gCamera;
    for (i = 0; i <= 1; i++) {
        gDisplayTable[i]->x = (cam[i].x >> 8) & 0x1ff;
        gDisplayTable[i]->y = (cam[i].y >> 8) & 0x1ff;
    }
}
