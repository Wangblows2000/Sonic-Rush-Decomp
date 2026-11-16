#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02054ac4;
typedef struct { s32 m[5]; } UnkMtx_02054ac4;
extern UnkMosaic_02054ac4 gDisplayMain;
extern UnkMosaic_02054ac4 gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];

void func_02000cb4(UnkMtx_02054ac4 *m);
void func_02003d94(void *a, UnkMtx_02054ac4 *m, s32 b, s32 c, s32 d, s32 e);

#pragma thumb on
void func_02054ac4(void) {
    UnkMtx_02054ac4 m;

    func_02000cb4(&m);
    gDisplayMain.v[0] = 0;
    gDisplayMain.v[1] = 0;
    gDisplayMain.v[2] = 0;
    gDisplayMain.v[3] = 0;
    gDisplayMain.v[4] = 0;
    gDisplayMain.v[5] = 0;
    gDisplayMain.v[6] = 0;
    gDisplayMain.v[7] = 0;
    func_02003d94(data_02089254, &m, 0, 0, 0, 0);
    func_02003d94(data_02089264, &m, 0, 0, 0, 0);
    gDisplaySub.v[0] = 0;
    gDisplaySub.v[1] = 0;
    gDisplaySub.v[2] = 0;
    gDisplaySub.v[3] = 0;
    gDisplaySub.v[4] = 0;
    gDisplaySub.v[5] = 0;
    gDisplaySub.v[6] = 0;
    gDisplaySub.v[7] = 0;
    func_02003d94(data_02087d2c, &m, 0, 0, 0, 0);
    func_02003d94(data_02087d3c, &m, 0, 0, 0, 0);
}
#pragma thumb reset
