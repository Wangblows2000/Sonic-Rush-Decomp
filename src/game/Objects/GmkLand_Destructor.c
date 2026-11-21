/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a05c[];
extern u8 data_ov000_0236a060[];
s32 MapObject_Destructor();

void GmkLand_Destructor(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1e0) + 0x2);
    s32 v1 = t0;
    if (t0 == 0xaf) {
        s32 t2 = *(s16 *)data_ov000_0236a05c;
        s32 v3 = t2;
        if (t2 != 0) {
            *(u16 *)data_ov000_0236a05c = (v3 - 0x1);
            v3 = (v3 - 0x1);
        }
        s32 t4 = *(s16 *)data_ov000_0236a05c;
        s32 v5 = t4;
        if (t4 == 0) {
            *(u32 *)data_ov000_0236a060 = 0;
            v5 = (s32)data_ov000_0236a060;
        }
        v1 = v5;
    }
    MapObject_Destructor(p0, v1);
}
