/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
s32 func_ov000_023355d0();
s32 NetPacket_Queue();

void MapObject_Unk_ov000_022fa9e8(u8 *p0, u8 *p1, s32 p2) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1e0);
    if (t0 != 0) {
        if (*(u8 *)t0 == 0xff) {
            if (*(u8 *)((u8 *)t0 + 0x1) == 0xff) {
                return;
            }
        }
    }
    if ((*(u32 *)((u8 *)gGameState + 0x18) & 0x20) == 0) {
        return;
    }
    if (*(u8 *)((u8 *)p1 + 0x325) != 0) {
        return;
    }
    s32 t1 = *(u8 *)((u8 *)p1 + 0x325);
    *(u8 *)((u8 *)p0 + 0x208) = p2;
    s32 t2 = func_ov000_023355d0(t1, p1, p2, t0);
    *(u16 *)((u8 *)p0 + 0x20a) = (*(u32 *)((u8 *)p0 + 0x1e0) - t2);
    NetPacket_Queue(((u8 *)p0 + 0x208), 2, 0, 4);
}
