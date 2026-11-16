#include <nitro/types.h>

typedef struct { u8 pad[0x68]; void *unk68; u8 pad6C[0x20C - 0x6C]; } UnkStruct_StageObj;

typedef struct {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_Mdl;

typedef struct {
    u8 pad[0x238];
    void *unk238;
    void *unk23C;
    void *unk240;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x454 - 0x20C];
    u16 unk454;
} UnkStruct_022f0cb0;

extern void *gMainHeap;
extern UnkStruct_Boss *data_ov000_02369b84;
extern UnkStruct_Mdl data_ov000_0236a524;
extern u8 data_ov000_02365f20[];

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(UnkStruct_Mdl *m);
void func_0202566c(void *p);
void *File_Load(void *p);
void func_020257f8(void *p);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, void *a, s32 b);
void func_ov000_022f0c8c(void);

static inline void UnkFree(void **pp) {
    void *p;
    u32 size;
    p = *pp;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    *pp = NULL;
}

void func_ov000_022f0cb0(UnkStruct_022f0cb0 *s) {
    UnkStruct_Boss *b = data_ov000_02369b84;
    UnkStruct_Mdl *m = &data_ov000_0236a524;

    s->unk454 = 0;
    Model_Release(m);
    func_0202566c(b->unk238);
    UnkFree(&b->unk238);
    func_0202566c(b->unk240);
    UnkFree(&b->unk240);
    b->unk23C = File_Load(data_ov000_02365f20);
    func_020257f8(b->unk23C);
    Model_Init(m, 0);
    Model_SetResource(m, b->unk23C, 0);
    m->unk18 = m->unk1C = m->unk20 = 0x1355b50;
    s->base.unk68 = func_ov000_022f0c8c;
}
