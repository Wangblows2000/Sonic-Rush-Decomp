/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
extern u8 data_ov000_0236a2fc[];
s32 MapObject_Destructor();

void GmkTable_Destructor(u8 *p0) {
    NNS_SndPlayerStopSeq(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1dc), 0);
    *(u32 *)data_ov000_0236a2fc = 0;
    MapObject_Destructor(p0, data_ov000_0236a2fc, 0);
}
