/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BrightnessFade_IsDone();
void TransformDemo_FadeState_WaitEnd();

void TransformDemo_FadeState_FadeIn(u8 *p0, u8 *p1) {
    s32 t0 = BrightnessFade_IsDone();
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) & ~0x20);
        *(u32 *)p1 = (u32)TransformDemo_FadeState_WaitEnd;
    }
}
