#include <nitro/types.h>

typedef struct UnkStruct_02300214 {
    u8 pad0[8];
    void *task8;
    void *taskC;
    void *task10;
    void *task14;
    u8 pad18[0x60 - 0x18];
    s8 unk60;
    s8 unk61;
    u8 pad62[0x33c - 0x62];
    u8 unk33C[4];
} UnkStruct_02300214;

typedef struct UnkStruct_02300214_s {
    u8 pad0[0x14];
    u8 unk14;
} UnkStruct_02300214_s;

extern UnkStruct_02300214 *gExtraBossWork;
extern UnkStruct_02300214_s data_022c4590;
extern s32 data_022c4560;
extern void *gMainHeap;

void PaletteTransfer_Clear(void);
void func_ov001_022f7274(void);
void func_ov001_02301668(void);
void func_ov001_023030dc(void);
void func_ov001_023053cc(void);
void func_ov001_02304568(void);
void func_ov001_022fdb04(void);
void func_ov001_022fd610(void);
void func_ov001_02306674(void);
void func_ov001_022ff8ac(void);
void Task_Destroy(void *t);
void func_0201bf1c(void *p);
void func_02059838(s32 a, s32 b);
s32 func_0205a4ec(s32 p0);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

void func_ov001_02300214(void) {
    vu32 fill;
    u8 *p;
    s32 size;

    PaletteTransfer_Clear();
    func_ov001_022f7274();
    func_ov001_02301668();
    func_ov001_023030dc();
    func_ov001_023053cc();
    func_ov001_02304568();
    func_ov001_022fdb04();
    func_ov001_022fd610();
    func_ov001_02306674();
    func_ov001_022ff8ac();
    Task_Destroy(gExtraBossWork->task8);
    Task_Destroy(gExtraBossWork->taskC);
    Task_Destroy(gExtraBossWork->task10);
    if (gExtraBossWork->task14 != NULL)
        Task_Destroy(gExtraBossWork->task14);
    func_0201bf1c(gExtraBossWork->unk33C);
    func_02059838(gExtraBossWork->unk60, gExtraBossWork->unk61);
    func_0205a4ec(0);
    p = (u8 *)gExtraBossWork;
    size = gExtraBossWork->unk61;
    if (size > 99)
        size = 99;
    else if (size < 2)
        size = 2;
    data_022c4560 = 1;
    data_022c4590.unk14 = size;
    if ((u8 *)func_02006abc(0) != p) {
        p -= 0x20;
        size = *(s32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    gExtraBossWork = NULL;
}
