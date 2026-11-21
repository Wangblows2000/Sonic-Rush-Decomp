/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDecSharedObjects[];

void DecShared_State_Main(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)gDecSharedObjects + (*(u32 *)((u8 *)p0 + 0x8) << 2)) = 0;
    }
    *(u32 *)((u8 *)p0 + 0xc) = 0;
}
