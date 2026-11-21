/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss4[];
void StageObj_Main();
void Boss4PartCE_Destructor();
s32 Task_Create();
s32 MI_CpuFill8();
s32 MapObject_Init();
void Boss4PartCE_Draw();
void Boss4PartCE_Collide();

s32 Boss4PartCE_Create(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)gBoss4;
    s32 t1 = Task_Create((u32)StageObj_Main, (u32)Boss4PartCE_Destructor, 0, 0x501, 2, 0x214);
    s32 t2 = *(u32 *)((u8 *)t1 + 0x10);
    MI_CpuFill8(*(u32 *)((u8 *)t1 + 0x10), 0, 0x214);
    MapObject_Init(t2, p0, p1, p2);
    *(u32 *)((u8 *)t2 + 0x70) = (u32)Boss4PartCE_Draw;
    *(u32 *)((u8 *)t2 + 0x68) = 0;
    *(u32 *)((u8 *)t2 + 0x78) = (u32)Boss4PartCE_Collide;
    *(u32 *)((u8 *)t2 + 0x54) = (*(u32 *)((u8 *)t2 + 0x54) | 0x10);
    *(u32 *)((u8 *)t2 + 0x58) = (*(u32 *)((u8 *)t2 + 0x58) | 0xa300);
    *(u32 *)((u8 *)t2 + 0x20c) = (t0 + 0x360);
    return t2;
}
