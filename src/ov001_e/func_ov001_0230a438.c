#include <nitro/types.h>

typedef struct UnkStruct_0230a438_o {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x48 - 0x38];
} UnkStruct_0230a438_o;

typedef struct UnkStruct_0230a438 {
    void *task0;
    void *task4;
    u8 pad8[0x108 - 0x8];
    UnkStruct_0230a438_o objs[0x17];
    UnkStruct_0230a438_o o780;
    UnkStruct_0230a438_o o7c8;
    UnkStruct_0230a438_o objs2[10];
    u8 padAE0[0xae8 - 0xae0];
    s32 unkAE8;
    u8 padAEC[0xbf4 - 0xaec];
    void *taskBF4;
} UnkStruct_0230a438;

extern UnkStruct_0230a438 *data_ov001_0231f5bc;
extern u32 data_ov001_02317144[];
extern void *gMainHeap;

void ObjVram_Free(u32 a, u32 b);
void Task_Destroy(void *task);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

void func_ov001_0230a438(void) {
    UnkStruct_0230a438 *g = data_ov001_0231f5bc;
    s32 i;
    u8 *blk;

    for (i = 0; i < 0x17; i++)
        ObjVram_Free(data_ov001_02317144[i], g->objs[i].unk34);
    if (g->unkAE8 != 0) {
        for (i = 0; i < 10; i++)
            ObjVram_Free(0, g->objs2[i].unk34);
        ObjVram_Free(0, g->o780.unk34);
        ObjVram_Free(0, g->o7c8.unk34);
    }
    if (g->task0 != NULL) {
        Task_Destroy(g->task0);
        g->task0 = NULL;
    }
    if (g->task4 != NULL) {
        Task_Destroy(g->task4);
        g->task4 = NULL;
    }
    if (g->taskBF4 != NULL) {
        Task_Destroy(g->taskBF4);
        g->taskBF4 = NULL;
    }
    blk = (u8 *)g;
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
    data_ov001_0231f5bc = NULL;
}
