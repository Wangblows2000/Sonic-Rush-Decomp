#include <nitro/types.h>

typedef struct UnkStruct_0231398c_v {
    s32 x, y, z;
} UnkStruct_0231398c_v;

typedef struct UnkStruct_0231398c_r {
    u8 pad0[0x18];
    UnkStruct_0231398c_v pos;
} UnkStruct_0231398c_r;

extern void *gMainHeap;
extern u8 *data_ov001_0231f694;
extern u8 data_ov001_0231eec0[];
extern u8 data_ov001_0231eed0[];
extern u8 data_ov001_0231eee0[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void func_ov001_023122d8(void *p, s32 v);

#define F32(o) (*(s32 *)(blk + (o)))
#define F16(o) (*(u16 *)(blk + (o)))
#define S16(o) (*(s16 *)(blk + (o)))
#define FP(o) (*(void **)(blk + (o)))

static inline void VecSet(UnkStruct_0231398c_v *d, s32 x, s32 y, s32 z) {
    d->x = x;
    d->y = y;
    d->z = z;
}

u8 *func_ov001_0231398c(u16 id) {
    u8 *s = data_ov001_0231f694;
    u8 *blk;

    blk = Heap_Alloc(gMainHeap, 0x260, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x260);
        *(u32 *)blk = 0x260;
        *(u32 *)(blk + 0x25c) = 0x02467531;
        DC_StoreRange(blk, 0x260);
        blk += 0x20;
    }
    MI_CpuFill8(blk, 0, 0x230);
    F16(0x16) = 0;
    F32(0x18) = 0;
    F32(0x1c) = 0;
    F32(0x20) = 0;
    F32(0x24) = -0x1e000;
    F32(0x28) = 0;
    F32(0x44) = 0;
    F32(0x48) = 0;
    F32(0x4c) = 0;
    F32(0x50) = 0;
    F32(0x54) = 0;
    F32(0x58) = 0;
    F32(4) = 0x1000;
    F32(8) = 0x1000;
    F32(0xc) = 0x1000;
    F16(0x10) = 0;
    F16(0x12) = 0x8000;
    F16(0x14) = 0;
    F16(0x5c) = 1;
    F16(0x5e) = 0;
    F16(0x60) = 0;
    F16(0x62) = 0;
    F16(0x64) = 0x2000;
    F16(0x66) = 0;
    F16(0x68) = id;
    F32(0x6c) = 0;
    F32(0x74) = 0;
    F32(0x78) = 0;
    FP(0xac) = s + 4;
    F32(0xb0) = 0;
    FP(0xb4) = data_ov001_0231eec0;
    F32(0xb8) = 1;
    FP(0xbc) = data_ov001_0231eed0;
    F32(0xc0) = 1;
    F32(0xcc) = 0;
    F32(0xd4) = 0;
    FP(0xd8) = blk;
    F32(0xdc) = 0;
    F32(0xe0) = 0;
    FP(0x12c) = data_ov001_0231eee0;
    F32(0x130) = 1;
    F32(0x134) = 0;
    F32(0x13c) = 0;
    F32(0x148) = 0;
    F16(0x14c) = 0;
    F16(0x14e) = 0;
    S16(0x150) = -1;
    S16(0x152) = -1;
    VecSet(&((UnkStruct_0231398c_r *)FP(0xac))->pos, F32(4), F32(8), F32(0xc));
    func_ov001_023122d8(blk, 3);
    return blk;
}
