/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
void GmkTable_State_Released();
s32 NNS_SndPlayerStopSeq();

void GmkTable_Action_Release(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x14) = (*(u16 *)((u8 *)gCamera + 0x62) << 8);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)GmkTable_State_Released;
    *(u16 *)((u8 *)p0 + 0x34) = 0x100;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x820000);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    *(u16 *)((u8 *)p0 + 0x428) = 0x100;
    NNS_SndPlayerStopSeq(*(u32 *)((u8 *)p0 + 0x1dc), 0, ((u8 *)p0 + 0x400), t0);
}
