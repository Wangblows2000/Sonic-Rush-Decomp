/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_G3dGlbPolygonAttr();
void BossFPartDE_State_Unk_ov000_022f6584();

void BossFPartDE_State_Unk_ov000_022f65fc(u8 *p0) {
    NNS_G3dGlbPolygonAttr(0, 0, 2, 0, 0xc, 0);
    *(u32 *)((u8 *)p0 + 0x4a0) = 1;
    *(u32 *)((u8 *)p0 + 0x4a4) = 1;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartDE_State_Unk_ov000_022f6584;
}
