#include <nitro/types.h>

typedef struct { u8 unk0[0x28]; u8 bg2[0x10]; u8 bg3[0x10]; } UnkDisp_0203039c;
typedef struct { u8 unk0[0x80]; u32 unk80; u32 unk84; } UnkStruct_0203039c;

extern u32 gSystemFlags;
extern u32 data_02087cc4;
extern u32 gSystemResetParameter;
extern u8 data_02087cdc[];
extern u32 gSaveDataBusy[];
extern UnkDisp_0203039c *gDisplayTable[];
extern u8 data_0208a754[];
extern UnkStruct_0203039c data_0208f754;
extern u32 gOverlayIsLoaded;
extern s32 gOverlayLoadedId;
extern s32 gOverlayRequestedId;
extern void *gOverlayCallback;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MTX_Identity22_(void *m);
void G2x_SetBGyAffine_(void *dst, const void *m, s32 a, s32 b, s32 c, s32 d);

void System_InitState(void) {
    vu16 z1;
    vu16 z2;
    vu16 z3;
    vu16 z4;
    fx32 m[4];
    u32 i;
    gSystemFlags = 0;
    data_02087cc4 = 0;
    gSystemResetParameter = *(u32 *)0x027FFC20;
    z1 = 0;
    MIi_CpuClear16(z1, data_02087cdc, 0x28);
    z2 = 0;
    MIi_CpuClear16(z2, gSaveDataBusy, 0x10);
    gSaveDataBusy[1] = 0;
    gSaveDataBusy[0] = 0;
    MTX_Identity22_(m);
    for (i = 0; i < 2; i++) {
        UnkDisp_0203039c *d;
        z3 = 0;
        d = gDisplayTable[i];
        MIi_CpuClear16(z3, d, 0x1528);
        G2x_SetBGyAffine_(d->bg2, m, 0, 0, 0, 0);
        G2x_SetBGyAffine_(d->bg3, m, 0, 0, 0, 0);
    }
    z4 = 0;
    MIi_CpuClear16(z4, data_0208a754, 0x5088);
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    gOverlayIsLoaded = 0;
    gOverlayLoadedId = -1;
    gOverlayRequestedId = -1;
    gOverlayCallback = NULL;
}
