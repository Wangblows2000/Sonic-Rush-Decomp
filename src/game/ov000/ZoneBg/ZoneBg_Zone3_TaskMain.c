#include <nitro/types.h>

typedef struct UnkStruct_02340034 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_02340034;

extern void *gZoneBgWork;
extern UnkStruct_02340034 gCamera;
extern u32 gSystemFlags;

void ZoneBg_Zone3_EnableLineScroll(void *p);
void ZoneBg_Zone3_BuildLineScroll(void *p, s32 a);
void ZoneBg_Zone3_BuildLineScrollOtherScreen(void *p);

void ZoneBg_Zone3_TaskMain(void) {
    void *p = gZoneBgWork;
    ZoneBg_Zone3_EnableLineScroll(p);
    if (gCamera.unkC8 & 1) {
        ZoneBg_Zone3_BuildLineScroll(p, 0);
        ZoneBg_Zone3_BuildLineScrollOtherScreen(p);
    } else {
        ZoneBg_Zone3_BuildLineScroll(p, 0);
        ZoneBg_Zone3_BuildLineScroll(p, 1);
    }
    gSystemFlags |= 0x3000;
}
