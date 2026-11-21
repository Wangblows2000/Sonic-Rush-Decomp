/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
extern u8 gCamera[];
s32 NetPacket_Queue();

void Player_SendNetState(u8 *p0) {
    if ((*(u32 *)((u8 *)gGameState + 0x18) & 0x20) == 0) {
        return;
    }
    if (*(u8 *)((u8 *)p0 + 0x325) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x48c) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x490) = *(u32 *)((u8 *)p0 + 0x14);
    *(u32 *)((u8 *)p0 + 0x494) = *(u32 *)((u8 *)p0 + 0x18);
    *(u8 *)((u8 *)p0 + 0x49c) = *(s16 *)((u8 *)p0 + 0x328);
    *(u16 *)((u8 *)p0 + 0x498) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x1000);
    *(u16 *)((u8 *)p0 + 0x49a) = *(s16 *)((u8 *)p0 + 0xbe);
    *(u8 *)((u8 *)p0 + 0x49d) = *(u8 *)((u8 *)p0 + 0x1);
    *(u8 *)((u8 *)p0 + 0x49e) = *(u8 *)((u8 *)p0 + 0x2);
    *(u8 *)((u8 *)p0 + 0x49f) = *(u8 *)((u8 *)p0 + 0x3);
    *(u16 *)((u8 *)p0 + 0x498) = *(u16 *)((u8 *)p0 + 0x5c);
    *(u32 *)((u8 *)p0 + 0x4a0) = *(u32 *)((u8 *)p0 + 0x58);
    *(u32 *)((u8 *)p0 + 0x4a4) = *(u32 *)((u8 *)p0 + 0x32c);
    *(u32 *)((u8 *)p0 + 0x4a8) = *(u32 *)((u8 *)p0 + 0x330);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x54) & 1);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x4a8) = (*(u32 *)((u8 *)p0 + 0x4a8) | 0x800000);
    }
    *(u16 *)((u8 *)p0 + 0x4ac) = *(s16 *)((u8 *)p0 + 0x28);
    *(u16 *)((u8 *)p0 + 0x4ae) = *(s16 *)((u8 *)p0 + 0x2a);
    *(u32 *)((u8 *)p0 + 0x4b0) = *(u32 *)((u8 *)gCamera + 0x4);
    *(u32 *)((u8 *)p0 + 0x4b4) = *(u32 *)((u8 *)gCamera + 0x8);
    *(u32 *)((u8 *)p0 + 0x4b8) = *(u32 *)((u8 *)gGameState + 0x20);
    NetPacket_Queue(((u8 *)p0 + 0x48c), 1, 3, 0x30);
}
