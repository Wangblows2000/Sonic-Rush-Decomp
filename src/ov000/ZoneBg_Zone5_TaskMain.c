#include <nitro/types.h>

typedef struct UnkStruct_0233f67c {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233f67c;

extern void *gZoneBgWork;
extern UnkStruct_0233f67c gCamera;
extern u32 gSystemFlags;

void ZoneBg_Zone5_EnableLineScroll(void *p);
void ZoneBg_Zone5_BuildLineScroll(void *p, s32 a);
void ZoneBg_Zone5_BuildLineScrollOtherScreen(void *p);

void ZoneBg_Zone5_TaskMain(void) {
    void *p = gZoneBgWork;
    ZoneBg_Zone5_EnableLineScroll(p);
    if (gCamera.unkC8 & 1) {
        ZoneBg_Zone5_BuildLineScroll(p, 0);
        ZoneBg_Zone5_BuildLineScrollOtherScreen(p);
    } else {
        ZoneBg_Zone5_BuildLineScroll(p, 0);
        ZoneBg_Zone5_BuildLineScroll(p, 1);
    }
    gSystemFlags |= 0x3000;
}
