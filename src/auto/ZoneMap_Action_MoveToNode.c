/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_GetPathIndex();
void ZoneMap_State_MoveToNode();

void ZoneMap_Action_MoveToNode(s32 p0) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    *(u32 *)((u8 *)t0 + 0x18) = p0;
    s32 t1 = ZoneMap_GetPathIndex(*(u32 *)t0, *(u32 *)((u8 *)t0 + 0x14), *(u32 *)((u8 *)t0 + 0x18));
    *(u32 *)((u8 *)t0 + 0x1c) = t1;
    s32 t2 = *(u32 *)gTaskCurrent;
    *(u32 *)((u8 *)t2 + 0x8) = (u32)ZoneMap_State_MoveToNode;
}

#pragma thumb reset
