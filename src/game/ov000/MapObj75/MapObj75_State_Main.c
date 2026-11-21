/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Camera_CheckPosInView();

void MapObj75_State_Main(u8 *p0) {
    Camera_CheckPosInView(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = *(u32 *)((u8 *)p0 + 0x234);
    if (t0 != 0) {
        ((s32 (*)())t0)(p0, 0, t0);
    }
    if (*(u32 *)((u8 *)p0 + 0x238) == 0) {
        return;
    }
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x238))(p0, 1, *(u32 *)((u8 *)p0 + 0x238));
}
