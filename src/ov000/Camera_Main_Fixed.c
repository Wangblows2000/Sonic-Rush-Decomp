#include <nitro/types.h>

typedef struct UnkStruct_02339db8 {
    u8 pad[0xc8];
    s32 unkC8;
    s32 unkCC;
} UnkStruct_02339db8;

extern UnkStruct_02339db8 gCamera;

void ZoneFx_Update(void);

void Camera_Main_Fixed(void) {
    gCamera.unkCC = gCamera.unkC8;
    ZoneFx_Update();
}
