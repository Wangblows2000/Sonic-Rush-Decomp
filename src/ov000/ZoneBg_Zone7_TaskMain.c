#include <nitro/types.h>

typedef struct UnkStruct_0233efb0 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233efb0;

extern void *gZoneBgWork;
extern UnkStruct_0233efb0 gCamera;
extern u32 gSystemFlags;

void ZoneBg_Zone7_EnableLineScroll(void *p);
void ZoneBg_Zone7_BuildLineScroll(void *p, s32 a);
void ZoneBg_Zone7_BuildLineScrollOtherScreen(void *p);

void ZoneBg_Zone7_TaskMain(void) {
    void *p = gZoneBgWork;
    ZoneBg_Zone7_EnableLineScroll(p);
    if (gCamera.unkC8 & 1) {
        ZoneBg_Zone7_BuildLineScroll(p, 0);
        ZoneBg_Zone7_BuildLineScrollOtherScreen(p);
    } else {
        ZoneBg_Zone7_BuildLineScroll(p, 0);
        ZoneBg_Zone7_BuildLineScroll(p, 1);
    }
    gSystemFlags |= 0x3000;
}
