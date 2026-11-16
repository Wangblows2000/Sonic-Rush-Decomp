#include <nitro/types.h>

typedef struct UnkStruct_0230de20_e {
    u8 pad0[0x3c];
    u8 anim[8];
    u16 unk44;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 pad49[0x138 - 0x49];
} UnkStruct_0230de20_e;

typedef struct UnkStruct_0230de20_a {
    u8 pad0[0x138];
} UnkStruct_0230de20_a;

extern void *gMainHeap;
extern u8 *data_ov001_0231f654;
extern void *data_ov001_0231e984[][4];
extern u8 data_ov001_0231e9f4[];
extern s32 data_ov001_023171ec[];
extern s32 data_ov001_02317208[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_02000dfc(void *m);
s32 func_ov001_0230c0cc(void);
u8 *File_Load(void *p, s32 v);
void func_020257f8(void *p);
u32 func_020380d8(void *p);
void func_020380a0(void *src, void *dst);
void Heap_Free(void *heap, void *p);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void *func_02026274(void *a, s32 b);
void func_020221fc(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
void func_02022228(u32 a, u32 b, BOOL c);
void func_02025dac(void *p, s32 a, s32 b);
void func_02025e1c(void *p, s32 a, s32 b);

#define F32(o) (*(s32 *)(sl + (o)))
#define FP(o) (*(void **)(sl + (o)))
#define F16(o) (*(u16 *)(sl + (o)))
#define F8(o) (*(u8 *)(sl + (o)))

void func_ov001_0230de20(void) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    vu32 fill4;
    vu32 fill5;
    u8 *sl;
    u8 *blk;
    s32 k;
    u32 size;
    s32 idx;
    s32 j;
    u8 *res;
    u32 sz;
    s32 sub;
    UnkStruct_0230de20_a *an;
    void *m;
    UnkStruct_0230de20_e *e;
    s32 i;

    blk = Heap_Alloc(gMainHeap, 0xb00, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, 0xb00);
        *(u32 *)blk = 0xb00;
        *(u32 *)(blk + 0xafc) = 0x02467531;
        DC_StoreRange(blk, 0xb00);
        blk += 0x20;
    }
    data_ov001_0231f654 = blk;
    MI_CpuFill8(blk, 0, 0xac8);
    sl = data_ov001_0231f654;
    blk = Heap_Alloc(gMainHeap, 0x240, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, blk, 0x240);
        *(u32 *)blk = 0x240;
        *(u32 *)(blk + 0x23c) = 0x02467531;
        DC_StoreRange(blk, 0x240);
        blk += 0x20;
    }
    FP(0xa40) = blk;
    blk = Heap_Alloc(gMainHeap, 0x60, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill3 = 0xcccccccc;
        MIi_CpuClear32(fill3, blk, 0x60);
        *(u32 *)blk = 0x60;
        *(u32 *)(blk + 0x5c) = 0x02467531;
        DC_StoreRange(blk, 0x60);
        blk += 0x20;
    }
    FP(0xa44) = blk;
    MI_CpuFill8(FP(0xa40), 0, 0x200);
    MI_CpuFill8(FP(0xa44), 0, 0x20);
    F32(0x38) = 0x12c05e;
    F32(0) = 0;
    F32(4) = 0;
    F32(0xa10) = 1;
    func_02000dfc(sl + 0xa14);
    for (j = 0; j < 4; j++) {
        res = File_Load(data_ov001_0231e984[func_ov001_0230c0cc()][j], -1);
        func_020257f8(res);
        sz = (func_020380d8(res) + 0x43) & ~0x1f;
        blk = Heap_Alloc(gMainHeap, sz, 0x20);
        if ((u8 *)func_02006abc(0) != blk) {
            fill4 = 0xcccccccc;
            MIi_CpuClear32(fill4, blk, sz);
            *(u32 *)blk = sz;
            *(u32 *)(blk + sz - 4) = 0x02467531;
            DC_StoreRange(blk, sz);
            blk += 0x20;
        }
        ((void **)(sl + 0x9fc))[j] = blk;
        func_020380a0(res, ((void **)(sl + 0x9fc))[j]);
        if ((u8 *)func_02006abc(0) != res) {
            res -= 0x20;
            sz = *(u32 *)res;
            if (*(u32 *)(res + sz - 4) == 0x02467531) {
                fill5 = 0xcccccccc;
                MIi_CpuClear32(fill5, res, sz);
                DC_StoreRange(res, sz);
            }
        }
        Heap_Free(gMainHeap, res);
    }
    FP(0xa0c) = File_Load(data_ov001_0231e9f4, -1);
    e = (UnkStruct_0230de20_e *)sl;
    an = (UnkStruct_0230de20_a *)(sl + 0x3c);
    for (i = 0; i < 7; i++) {
        idx = data_ov001_023171ec[i];
        sub = data_ov001_02317208[i];
        Model_Init(an, 0);
        Model_SetResource(an, ((void **)(sl + 0x9fc))[idx], sub);
        e->unk46 = 4;
        e->unk47 = 2;
        e->unk44 = 0x1e;
        m = func_02026274(((void **)(sl + 0x9fc))[idx], sub);
        func_020221fc(0, 0, 3, 0, 0x1f, 0x8000);
        func_02022228(0, 0x7fff, 0);
        func_02025dac(m, 0, 0xf);
        func_02025e1c(m, 0, 0x400);
        func_02025dac(m, 0, 0xc0);
        func_02025dac(m, 0, 0x8000);
        an++;
        e++;
    }
    Model_Init(sl + 0x8c4, 0);
    Model_SetResource(sl + 0x8c4, FP(0xa08), 0);
    Model_SetAnimation(sl + 0x8c4, 3, FP(0xa0c), 0, 0);
    F8(0x9d3) |= 4;
    F32(0x8dc) = 0x320000;
    F32(0x8e0) = 0x320000;
    F32(0x8e4) = 0x320000;
    F8(0x8ce) = 0x13;
    F8(0x8cf) = 3;
    F8(0x8d0) = 0;
    m = func_02026274(FP(0xa08), 0);
    func_020221fc(0, 0, 3, 0, 0x1f, 0x8000);
    func_02022228(0, 0x7fff, 0);
    func_02025dac(m, 0, 0xf);
    func_02025e1c(m, 0, 0x400);
    func_02025dac(m, 0, 0x8000);
    F16(0xa38) = 8;
    F16(0xa3a) = 0;
    F16(0xa3c) = 0;
    F16(0xa3e) = 0;
    F32(0xa48) = 0;
    FP(0xa80) = sl + 0xa88;
    F32(0xa84) = 0;
    F32(0xac0) = 0;
    FP(0xac4) = sl + 0xa48;
    for (k = 0; k < F16(0xa38); k++)
        ((u8 **)FP(0xa44))[k] = (u8 *)FP(0xa40) + k * 0x40;
}
