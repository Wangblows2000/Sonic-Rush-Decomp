/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
void Camera_Look_Idle();

void Camera_Look_Reset(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)gCamera + 0xc8) & ~0x10000);
    *(u32 *)((u8 *)gCamera + 0xc8) = t0;
    *(u32 *)((u8 *)gCamera + 0xc8) = (t0 & ~0x20000);
    *(u32 *)p0 = (*(u32 *)p0 & ~2);
    *(u32 *)p0 = (*(u32 *)p0 & ~4);
    *(u16 *)((u8 *)p0 + 0x6) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = (u32)Camera_Look_Idle;
}
