/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b2c[];
void StageObj_Main();
void Boss4PartCF_Destructor();
s32 Task_Create();
s32 MI_CpuFill8();
s32 MapObject_Init();
void Boss4PartCF_Draw();
void Boss4PartCF_Collide();

s32 Boss4PartCF_Create(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)data_ov000_02369b2c;
    s32 t1 = Task_Create((u32)StageObj_Main, (u32)Boss4PartCF_Destructor, 0, 0x502, 2, 0x214);
    s32 t2 = *(u32 *)((u8 *)t1 + 0x10);
    MI_CpuFill8(*(u32 *)((u8 *)t1 + 0x10), 0, 0x214);
    MapObject_Init(t2, p0, p1, p2);
    *(u32 *)((u8 *)t2 + 0x70) = (u32)Boss4PartCF_Draw;
    *(u32 *)((u8 *)t2 + 0x68) = 0;
    *(u32 *)((u8 *)t2 + 0x78) = (u32)Boss4PartCF_Collide;
    *(u32 *)((u8 *)t2 + 0x54) = (*(u32 *)((u8 *)t2 + 0x54) | 0x10);
    *(u32 *)((u8 *)t2 + 0x58) = (*(u32 *)((u8 *)t2 + 0x58) | 0xa300);
    *(u32 *)((u8 *)t2 + 0x20c) = (t0 + 0x498);
    return t2;
}
