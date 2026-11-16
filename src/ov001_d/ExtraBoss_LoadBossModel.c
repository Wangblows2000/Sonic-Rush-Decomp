#include <nitro/types.h>

typedef struct UnkStruct_022eeab0_arc {
    u8 pad[0x60];
} UnkStruct_022eeab0_arc;

extern void *gMainHeap;
extern u8 *data_ov001_0231f070;
extern void *data_ov001_0231f528;
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern char data_ov001_023167a8[];
extern char data_ov001_0231d450[];

u8 *File_Load(const char *path, s32 v);
BOOL func_020257f8(void *res);
u32 func_020380d8(void *s);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020380a0(void *src, void *dst);
void Heap_Free(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
void *func_02021264(void *arc, u32 idx);
BOOL func_020212e0(void *arc);

void ExtraBoss_LoadBossModel(void) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_022eeab0_arc arc;
    u8 *res;
    u32 size;
    u8 *blk;

    res = File_Load(data_ov001_023167a8, -1);
    func_020257f8(res);
    size = (func_020380d8(res) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    data_ov001_0231f070 = blk;
    func_020380a0(res, blk);
    if ((u8 *)func_02006abc(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);
    func_02021310(&arc, data_ov001_0231d450, data_ov001_0231f528);
    data_ov001_0231f068 = func_02021264(&arc, 0x28);
    data_ov001_0231f074 = func_02021264(&arc, 0x29);
    func_020212e0(&arc);
}
