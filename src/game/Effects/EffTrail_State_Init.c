/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 EffTrail_State_Follow();
s32 EffTrail_ApplyWrapShift();
s32 EffTrail_SetNodeFromOwner();
void EffTrail_State_Wait();
void EffTrail_Draw();

void EffTrail_State_Init(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x134);
    if (*(u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x134) == 1) {
        if ((*(u32 *)((u8 *)t1 + 0x32c) & 0x80) == 0) {
            *(u32 *)((u8 *)t0 + 0x68) = (u32)EffTrail_State_Follow;
            EffTrail_State_Follow((u32)EffTrail_State_Follow);
            return;
        }
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x29c);
    s32 t3 = *(u32 *)((u8 *)*(u32 *)((u8 *)t0 + 0x29c) + 0x18);
    *(u32 *)((u8 *)t0 + 0x29c) = t3;
    EffTrail_ApplyWrapShift(t0, t2);
    EffTrail_SetNodeFromOwner(t3, t1, *(u32 *)((u8 *)t0 + 0x138));
    *(u32 *)((u8 *)t0 + 0x2a0) = (*(u32 *)((u8 *)t0 + 0x2a0) + 0x1);
    s32 t4 = *(u32 *)((u8 *)t0 + 0x2a0);
    s32 t5 = *(u32 *)((u8 *)t0 + 0x2a4);
    if (t4 >= t5) {
        *(u32 *)((u8 *)t0 + 0x2a0) = t5;
        *(u32 *)((u8 *)t0 + 0x68) = (u32)EffTrail_State_Wait;
    }
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)t0 + 0x70) = (u32)EffTrail_Draw;
}
