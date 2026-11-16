#include <nitro/types.h>

typedef struct UnkStruct_02314e7c_e {
    u8 unk0;
    u8 pad1[0x58 - 1];
} UnkStruct_02314e7c_e;

extern void *gMainHeap;
extern s32 data_ov001_0231f6a8;
extern UnkStruct_02314e7c_e *data_ov001_0231f6a4;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);

static inline u8 *Alloc(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void func_ov001_02314e7c(s32 n) {
    s32 i;
    u32 size = (n * 0x58 + 0x43) & ~0x1f;

    data_ov001_0231f6a8 = n;
    data_ov001_0231f6a4 = (UnkStruct_02314e7c_e *)Alloc(size);
    MI_CpuFill8(data_ov001_0231f6a4, 0, n * 0x58);
    for (i = 0; i < n; i++)
        data_ov001_0231f6a4[i].unk0 = 0;
}
