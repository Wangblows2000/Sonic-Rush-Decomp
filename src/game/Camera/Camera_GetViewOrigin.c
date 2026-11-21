/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Camera_GetViewOrigin(u8 *p0, u8 *p1, u8 *p2) {
    *(u32 *)p1 = ((*(u32 *)((u8 *)p0 + 0x4) + *(s16 *)((u8 *)p0 + 0x14)) + (*(s16 *)((u8 *)p0 + 0x18) << 8));
    s32 t0 = ((*(u32 *)((u8 *)p0 + 0x8) + *(s16 *)((u8 *)p0 + 0x16)) + (*(s16 *)((u8 *)p0 + 0x1a) << 8));
    *(u32 *)p2 = t0;
}
