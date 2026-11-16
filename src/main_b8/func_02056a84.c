#include <nitro/types.h>

typedef struct {
    u8 *buf;
} UnkWork_02056a84;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02056a84 *work;
} UnkTask_02056a84;

extern UnkTask_02056a84 *gTaskCurrent;
extern void *gMainHeap;

u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Task_Destroy(UnkTask_02056a84 *t);

#pragma thumb on
void func_02056a84(void) {
    u8 *p = gTaskCurrent->work->buf;

    if (p != NULL) {
        if ((u8 *)func_02006abc(0) != p) {
            u32 size;
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x02467531) {
                vu32 fill = 0xcccccccc;
                MIi_CpuClear32(fill, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
    }
    Task_Destroy(gTaskCurrent);
}
#pragma thumb reset
