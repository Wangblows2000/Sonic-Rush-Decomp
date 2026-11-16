#include <nitro/types.h>

typedef struct UnkStruct_02302be8_a {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_02302be8_a;

typedef struct UnkStruct_02302be8_s {
    u8 pad0[4];
    void (*unk4)(void);
    u32 unk8;
    u8 padC[0x38 - 0xc];
} UnkStruct_02302be8_s;

typedef struct UnkStruct_02302be8_d {
    u8 pad0[8];
    void *unk8;
    s32 unkC;
} UnkStruct_02302be8_d;

typedef struct UnkStruct_02302be8_o {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x74 - 0x30];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_02302be8_a *unk84;
    UnkStruct_02302be8_a *unk88;
    UnkStruct_02302be8_a *unk8C;
    u8 pad90[0xb0 - 0x90];
    UnkStruct_02302be8_s subB0;
    u8 padE8[0x1dc - 0xe8];
    UnkStruct_02302be8_d *unk1DC;
} UnkStruct_02302be8_o;

typedef struct UnkStruct_02302be8_p {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5c - 0x1c];
    u16 unk5C;
    u8 pad5E[0x134 - 0x5e];
    u8 unk134[0x138];
} UnkStruct_02302be8_p;

typedef struct UnkStruct_02302be8_t {
    u8 pad[0x18];
} UnkStruct_02302be8_t;

extern void *gMainHeap;
extern void *data_ov001_0231d9f0[];
extern void *data_ov001_0231d9f8[];
extern u8 *data_ov001_0231f540;
extern u8 *data_ov001_0231f538;
extern UnkStruct_02302be8_t data_ov001_0231da50[];
extern s32 data_ov001_0231d9e8[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u8 *File_Load(void *p, s32 v);
void NNS_G3dResDefaultSetup(void *p);
u32 Asset3D_GetResourceSize(void *p);
void Asset3D_CopyResource(void *src, void *dst);
void Heap_Free(void *heap, void *p);
UnkStruct_02302be8_o *ExtraBossObj_Create(s32 a, s32 b);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void ExtraBossPlayer_State_None(void);
void func_ov001_02301a88(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ExtraBossPlayer_ChangeState(UnkStruct_02302be8_o *o, s32 a);
u8 *ExtraBoss_GetWork(void);

void ExtraBoss_LoadPlayerModel(s32 idx) {
    vu16 zero;
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    vu32 fill4;
    vu32 fill5;
    vu32 fill6;
    vu32 fill7;
    u8 *res;
    u32 size;
    u8 *blk;
    UnkStruct_02302be8_o *o;
    UnkStruct_02302be8_a *a;
    UnkStruct_02302be8_d *d;
    UnkStruct_02302be8_p *p;

    res = File_Load(data_ov001_0231d9f0[idx], -1);
    NNS_G3dResDefaultSetup(res);
    size = (Asset3D_GetResourceSize(res) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    data_ov001_0231f540 = blk;
    Asset3D_CopyResource(res, blk);
    if ((u8 *)OS_GetArenaLo(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);

    res = File_Load(data_ov001_0231d9f8[idx], -1);
    NNS_G3dResDefaultSetup(res);
    size = (Asset3D_GetResourceSize(res) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill3 = 0xcccccccc;
        MIi_CpuClear32(fill3, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    data_ov001_0231f538 = blk;
    Asset3D_CopyResource(res, blk);
    if ((u8 *)OS_GetArenaLo(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill4 = 0xcccccccc;
            MIi_CpuClear32(fill4, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);

    o = ExtraBossObj_Create(0, 0x3000);
    a = (UnkStruct_02302be8_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill5 = 0xcccccccc;
        MIi_CpuClear32(fill5, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_02302be8_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f540, 0);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk84 = a;
    o->unk88 = a;
    a = (UnkStruct_02302be8_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill6 = 0xcccccccc;
        MIi_CpuClear32(fill6, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_02302be8_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f538, 0);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk8C = a;
    o->unk74 = ExtraBossPlayer_State_None;
    o->unk14 |= 0x1b0020;
    d = (UnkStruct_02302be8_d *)Heap_Alloc(gMainHeap, 0x40, 0x20);
    if ((void *)OS_GetArenaLo(0) != d) {
        fill7 = 0xcccccccc;
        MIi_CpuClear32(fill7, d, 0x40);
        *(u32 *)d = 0x40;
        *(u32 *)((u8 *)d + 0x3c) = 0x02467531;
        DC_StoreRange(d, 0x40);
        d = (UnkStruct_02302be8_d *)((u8 *)d + 0x20);
    }
    zero = 0;
    MIi_CpuClear16(zero, d, 0x14);
    o->unk1DC = d;
    d->unk8 = &data_ov001_0231da50[idx];
    d->unkC = data_ov001_0231d9e8[idx];
    func_ov001_022fe8d4(&o->subB0, o, 1, 0, 0x8000, 0x8000, 0x8000, 0, -0x8000, 0, 0);
    o->subB0.unk4 = func_ov001_02301a88;
    o->subB0.unk8 |= 0x2000;
    ExtraBossPlayer_ChangeState(o, 1);
    o->unk18 = o->unk24 = 0x40000;
    o->unk1C = o->unk28 = 0xc0000;
    o->unk20 = o->unk2C = 0;
    p = (UnkStruct_02302be8_p *)(ExtraBoss_GetWork() + 0x64);
    p->unk5C |= 4;
    p->unk10 = 0x40;
    p->unk14 = 0x60;
    p->unk18 = o->unk20 >> 12;
    MIi_CpuCopy16(o->unk84, p->unk134, 0x138);
}
