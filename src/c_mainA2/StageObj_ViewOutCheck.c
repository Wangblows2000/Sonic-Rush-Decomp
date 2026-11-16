#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkVec2_02068e58;

typedef struct {
    u32 unk0;
    UnkVec2_02068e58 a;
    u8 padc[0x68 - 0xc];
    UnkVec2_02068e58 b;
} UnkCam_02068e58;

extern s16 data_0207cf80[2];
extern u32 gObjManagerFlags;
extern UnkCam_02068e58 gCamera;

BOOL StageObj_ViewOutCheck(fx32 x, fx32 y, s16 offset, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom)
{
    s32 testLeft, testTop;
    s32 testRight, testBottom;

    s16 screenWidth = 0x100, screenHeight = 0xc0;

    if (data_0207cf80[0] != 0x100)
        screenWidth = (s16)(0x200 - data_0207cf80[0]);
    if (data_0207cf80[1] != 0x100)
        screenHeight = (s16)((screenHeight * (0x200 - data_0207cf80[1])) >> 8);

    if (offset == 0)
        offset = 0x40;

    if ((gObjManagerFlags & 4) == 0)
        return FALSE;

    if (gObjManagerFlags & 0x200) {
        if (gCamera.a.y < gCamera.b.y) {
            testLeft = (gCamera.a.x >> 8) - offset;
            testTop = (gCamera.a.y >> 8) - offset;
            testRight = screenWidth + (offset << 1);
            testBottom = ((gCamera.b.y >> 8) - (gCamera.a.y >> 8)) + screenHeight + (offset << 1);
        } else {
            testLeft = (gCamera.b.x >> 8) - offset;
            testTop = (gCamera.b.y >> 8) - offset;
            testRight = screenWidth + (offset << 1);
            testBottom = ((gCamera.a.y >> 8) - (gCamera.b.y >> 8)) + screenHeight + (offset << 1);
        }

        testLeft += sLeft;
        testTop += sTop;
        testBottom += -sTop + sBottom;
        testRight += -sLeft + sRight;

        if ((testLeft <= (x >> 8) && (testLeft + testRight >= (x >> 8))) && (testTop <= (y >> 8) && testTop + testBottom >= (y >> 8)))
            return FALSE;
    } else {
        testLeft = (gCamera.a.x >> 8) - offset;
        testTop = (gCamera.a.y >> 8) - offset;
        testRight = screenWidth + (offset << 1);
        testBottom = screenHeight + (offset << 1);

        testLeft += sLeft;
        testTop += sTop;
        testBottom += -sTop + sBottom;
        testRight += -sLeft + sRight;

        if ((testLeft <= (x >> 8) && testLeft + testRight >= (x >> 8)) && (testTop <= (y >> 8) && testTop + testBottom >= (y >> 8)))
            return FALSE;

        testLeft = (gCamera.b.x >> 8) - offset;
        testTop = (gCamera.b.y >> 8) - offset;

        testLeft += sLeft;
        testTop += sTop;

        if ((testLeft <= (x >> 8) && testLeft + testRight >= (x >> 8)) && (testTop <= (y >> 8) && testTop + testBottom >= (y >> 8)))
            return FALSE;
    }

    return TRUE;
}
