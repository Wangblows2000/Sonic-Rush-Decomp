/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gObjManagerFlags[];

s32 Player_Unk_ov000_0235258c(u8 *p0, s32 p1) {
    *(u32 *)((u8 *)p0 + 0x3e8) = p1;
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x1000);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 8);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x30);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x100);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x200000);
    *(u16 *)((u8 *)p0 + 0x420) = 0;
    *(u16 *)((u8 *)p0 + 0x422) = 0;
    *(u16 *)((u8 *)p0 + 0x424) = 0;
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x40000);
    *(u16 *)((u8 *)p0 + 0x3ec) = 0;
    *(u16 *)((u8 *)p0 + 0x3ee) = 0x58;
    s32 t0 = (*(u32 *)gObjManagerFlags & 0x100);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~0xc0);
    }
    *(u32 *)gObjManagerFlags = (*(u32 *)gObjManagerFlags | 0x100);
    return (s32)gObjManagerFlags;
}
