/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss1_UpdateCameraShake();
void Boss1Neck_State_TripleSlamStartRewind();

void Boss1Neck_State_TripleSlamImpactShake(u8 *p0) {
    Boss1_UpdateCameraShake(((u8 *)p0 + 0x20c), 2, 6);
    *(u16 *)((u8 *)p0 + 0x20e) = (*(u16 *)((u8 *)p0 + 0x20e) + 0x1);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x20e);
    if ((u32)t0 > (u32)4) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_TripleSlamStartRewind;
    }
}
