/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BrightnessFade_Create();
void ClearDemo_Create();
s32 StageShutdown_Create();

void Marker_State_Unk_ov000_02310418(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xc) == 0x10) {
        BrightnessFade_Create(4, 0x100);
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (t0 - 0x1);
        return;
    }
    StageShutdown_Create((u32)ClearDemo_Create);
}
