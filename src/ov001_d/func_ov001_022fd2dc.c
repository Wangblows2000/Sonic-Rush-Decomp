#include <nitro/types.h>

typedef struct UnkStruct_022fd2dc_d {
    u32 flags;
    u8 pad4[0x18];
} UnkStruct_022fd2dc_d;

typedef struct UnkStruct_022fd2dc_r {
    void *res;
    void *anims[5];
} UnkStruct_022fd2dc_r;

typedef struct UnkStruct_022fd2dc {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022fd2dc;

extern void *gMainHeap;
extern UnkStruct_022fd2dc_d data_ov001_0231d6b4[];
extern UnkStruct_022fd2dc_r data_ov001_0231f098[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Model_Init(void *s, u32 a);
BOOL func_020257f8(void *res);
void Model_SetResource(void *s, void *res, u32 idx);
u32 func_02025e8c(void *h);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);

UnkStruct_022fd2dc *func_ov001_022fd2dc(UnkStruct_022fd2dc *o, s32 n) {
    vu32 fill;
    s32 i;
    u32 tex;

    if (o == NULL) {
        u8 *blk = Heap_Alloc(gMainHeap, 0x160, 0x20);
        if ((u8 *)func_02006abc(0) != blk) {
            fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, 0x160);
            *(u32 *)blk = 0x160;
            *(u32 *)(blk + 0x15c) = 0x02467531;
            DC_StoreRange(blk, 0x160);
            blk += 0x20;
        }
        o = (UnkStruct_022fd2dc *)blk;
        Model_Init(o, 0);
    }
    if (!(data_ov001_0231d6b4[n].flags & 1))
        func_020257f8(data_ov001_0231f098[n].res);
    Model_SetResource(o, data_ov001_0231f098[n].res, 0);
    o->unk20 = 0x34cc;
    o->unk1C = o->unk20;
    o->unk18 = o->unk1C;
    tex = func_02025e8c(data_ov001_0231f098[n].res);
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f098[n].anims[i] != NULL)
            Model_SetAnimation(o, i, data_ov001_0231f098[n].anims[i], 0, tex);
    }
    return o;
}
