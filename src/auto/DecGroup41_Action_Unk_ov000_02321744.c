/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void DecGroup41_State_Unk_ov000_023216cc();

void DecGroup41_Action_Unk_ov000_02321744(u8 *p0, s32 p1, s32 p2, s32 p3) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)DecGroup41_State_Unk_ov000_023216cc;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x2e) = p1;
    *(u16 *)((u8 *)p0 + 0x30) = p2;
    *(u32 *)((u8 *)p0 + 0xc) = p3;
    if ((u32)*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x184) + 0x2) < (u32)0xab) {
        return;
    }
    if ((u32)*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x184) + 0x2) >= (u32)0xad) {
        return;
    }
    *(u16 *)((u8 *)p0 + 0x2e) = ((s32)*(s16 *)((u8 *)p0 + 0x2e) >> 1);
    *(u16 *)((u8 *)p0 + 0x30) = ((s32)*(s16 *)((u8 *)p0 + 0x30) >> 1);
}
