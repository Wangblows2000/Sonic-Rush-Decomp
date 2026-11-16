#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02054ac4;
typedef struct { s32 m[5]; } UnkMtx_02054ac4;
extern UnkMosaic_02054ac4 data_0208922c;
extern UnkMosaic_02054ac4 data_02087d04;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];

void func_02000cb4(UnkMtx_02054ac4 *m);
void func_02003d94(void *a, UnkMtx_02054ac4 *m, s32 b, s32 c, s32 d, s32 e);

#pragma thumb on
void func_ov000_0235f2d0(void) {
    UnkMtx_02054ac4 m;

    func_02000cb4(&m);
    data_0208922c.v[0] = 0;
    data_0208922c.v[1] = 0;
    data_0208922c.v[2] = 0;
    data_0208922c.v[3] = 0;
    data_0208922c.v[4] = 0;
    data_0208922c.v[5] = 0;
    data_0208922c.v[6] = 0;
    data_0208922c.v[7] = 0;
    func_02003d94(data_02089254, &m, 0, 0, 0, 0);
    func_02003d94(data_02089264, &m, 0, 0, 0, 0);
    data_02087d04.v[0] = 0;
    data_02087d04.v[1] = 0;
    data_02087d04.v[2] = 0;
    data_02087d04.v[3] = 0;
    data_02087d04.v[4] = 0;
    data_02087d04.v[5] = 0;
    data_02087d04.v[6] = 0;
    data_02087d04.v[7] = 0;
    func_02003d94(data_02087d2c, &m, 0, 0, 0, 0);
    func_02003d94(data_02087d3c, &m, 0, 0, 0, 0);
}
#pragma thumb reset
