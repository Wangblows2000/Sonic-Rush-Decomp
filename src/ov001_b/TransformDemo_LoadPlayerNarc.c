#include <nitro/types.h>

typedef struct UnkStruct_022df0b4_a {
    u8 pad0[0x10c];
    u8 unk10C;
} UnkStruct_022df0b4_a;

typedef struct UnkStruct_022df0b4 {
    s32 unk0;
    s32 unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    void *unk14;
    u8 pad18[0xc];
    s32 unk24;
    u8 pad28[0x14];
    s32 unk3C;
    u8 pad40[0x12];
    u16 unk52;
    u8 pad54[8];
    UnkStruct_022df0b4_a anim;
} UnkStruct_022df0b4;

typedef struct UnkStruct_022df0b4_c {
    u8 pad0[4];
    u32 unk4;
} UnkStruct_022df0b4_c;

typedef struct UnkStruct_022df0b4_arc {
    u8 pad[0x5c];
} UnkStruct_022df0b4_arc;

extern void *gMainHeap;
extern u8 data_ov001_0231c3cc[];
extern char data_ov001_0231c3e4[];
extern char data_ov001_0231c3e8[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u8 *File_Load(void *p, s32 v);
u32 func_ov001_022de6f8(u32 *p);
u32 func_ov001_022de6ec(u32 *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
u32 *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
void *func_02021264(void *arc, u32 idx);
BOOL func_020212e0(void *arc);
void func_020257f8(void *p);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);

void TransformDemo_LoadPlayerNarc(UnkStruct_022df0b4_c *c, UnkStruct_022df0b4 *s) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_022df0b4_arc arc;
    u8 *res;
    u8 *dst;
    u32 size;
    u8 *blk;
    UnkStruct_022df0b4_a *a;

    res = File_Load(data_ov001_0231c3cc, -1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    s->unk8 = blk;
    dst = s->unk8;
    switch (func_ov001_022de6f8((u32 *)res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        Mem_Copy(Compress_GetData(res), dst, func_ov001_022de6ec((u32 *)res));
        break;
    }
    DC_StoreRange(dst, func_ov001_022de6ec((u32 *)res));
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
    if (s->unk0 == 0) {
        func_02021310(&arc, data_ov001_0231c3e4, s->unk8);
        s->unkC = func_02021264(&arc, 0);
        s->unk10 = func_02021264(&arc, 1);
        s->unk14 = func_02021264(&arc, 2);
        func_020212e0(&arc);
    } else {
        func_02021310(&arc, data_ov001_0231c3e8, s->unk8);
        s->unkC = func_02021264(&arc, 3);
        s->unk10 = func_02021264(&arc, 1);
        s->unk14 = func_02021264(&arc, 2);
        func_020212e0(&arc);
    }
    func_020257f8(s->unkC);
    a = &s->anim;
    Model_Init(a, 0);
    Model_SetResource(a, s->unkC, 0);
    Model_SetAnimation(a, 0, s->unk10, 0xf, 0);
    a->unk10C |= 4;
    Model_SetAnimation(a, 4, s->unk14, 1, 0);
    if (c->unk4 & 0x40000000) {
        s->unk52 = 0x1700;
        s->unk24 = 0;
    } else {
        s->unk52 = 0x1700;
        s->unk3C = 0x3000;
        s->unk24 = -0x190000;
    }
    s->unk4 = -0x18b000;
}
