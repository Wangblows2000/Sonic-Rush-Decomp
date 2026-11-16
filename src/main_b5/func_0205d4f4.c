#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 stage;
    u8 act;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    s16 scrollX;
    s16 scrollY;
} UnkDisp_0205d4f4;

typedef struct {
    u8 pad0[4];
    s32 x;
    s32 y;
} UnkCam_0205d4f4;

typedef struct { s32 m[4]; } UnkMtx22_0205d4f4;

extern UnkStruct_022c4560 data_022c4560;
extern UnkDisp_0205d4f4 gDisplayMain;
extern UnkDisp_0205d4f4 gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02087d2c[];

void func_02000cb4(UnkMtx22_0205d4f4 *m);
void func_02003d94(void *p, UnkMtx22_0205d4f4 *m, s32 x, s32 y, s32 a, s32 b);

void func_0205d4f4(u8 *w) {
    UnkCam_0205d4f4 *cam = (UnkCam_0205d4f4 *)(w + 0x78);
    BOOL special;
    UnkMtx22_0205d4f4 m;

    special = FALSE;
    if (data_022c4560.stage == 8 && data_022c4560.act == 2)
        special = TRUE;
    if (special || data_022c4560.stage >= 9 || data_022c4560.act >= 2) {
        gDisplayMain.scrollX = cam->x >> 12;
        gDisplayMain.scrollY = cam->y >> 12;
        gDisplaySub.scrollX = cam->x >> 12;
        gDisplaySub.scrollY = cam->y >> 12;
    } else {
        func_02000cb4(&m);
        func_02003d94(data_02089254, &m, 0x80, 0x60, cam->x >> 12, cam->y >> 12);
        func_02003d94(data_02087d2c, &m, 0x80, 0x60, cam->x >> 12, cam->y >> 12);
    }
}
