#include <nitro/types.h>

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 x;
    s32 y;
    u8 pad18[0x1F8 - 0x18];
    void *unk1F8;
    void *unk1FC;
    u8 pad200[0x325 - 0x200];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x384 - 0x352];
    u8 unk384[4];
    u8 unk388[4];
    u8 unk38C[4];
    u8 unk390[0x3AE - 0x390];
    s16 unk3AE;
    s16 unk3B0;
    u8 pad3B2[0x414 - 0x3B2];
    u8 unk414[0xC];
} UnkStruct_0234d384_P;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0234d384_P *ply;
} UnkStruct_0234d384;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_0234d384_4560;

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    u32 unk30;
    u32 unk34;
    u8 pad38[0x3A - 0x38];
    s16 unk3A;
    u32 unk3C;
} UnkStruct_0234d384_4590;

extern UnkStruct_0234d384_4560 data_022c4560;
extern UnkStruct_0234d384_4590 data_022c4590;
extern void *gMainHeap;
extern u8 gPlayerCount;

void func_0201bf1c(void *p);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_0206ce90(void *p);
void StageObj_Destructor(UnkStruct_0234d384 *t);

static inline void Free_0234d384(void *p) {
    u32 size;
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
}

void Player_Destructor(UnkStruct_0234d384 *t) {
    UnkStruct_0234d384_P *w = t->ply;

    func_0201bf1c(w->unk384);
    func_0201bf1c(w->unk388);
    func_0201bf1c(w->unk38C);
    func_0201bf1c(w->unk390);
    if ((data_022c4560.unk18 & 0x40) && w->unk325 == 0) {
        data_022c4590.unk28 = w->unk3AE;
        data_022c4590.unk2A = w->unk3B0;
        data_022c4590.unk3C = data_022c4560.unk20;
        data_022c4590.unk2C = 0;
        if (w->unk32C & 0x10000000) {
            data_022c4590.unk2C = 1;
        }
        if (w->unk32C & 0x20000000) {
            data_022c4590.unk2C = 2;
        }
        data_022c4590.unk2E = w->unk350;
        data_022c4590.unk30 = (u16)(w->x >> 8);
        if (w->unk5 != 0) {
            data_022c4590.unk34 = (u16)((w->y >> 8) - 0x118);
        } else {
            data_022c4590.unk34 = (u16)((w->y >> 8) + 0x118);
        }
        if (w->unk330 & 0x8000) {
            data_022c4590.unk3A = 1;
        } else {
            data_022c4590.unk3A = 0;
        }
    }
    if (w->unk1F8 != NULL) {
        Free_0234d384(w->unk1F8);
    }
    if (w->unk1FC != NULL) {
        Free_0234d384(w->unk1FC);
    }
    func_0206ce90(w->unk414);
    gPlayerCount--;
    StageObj_Destructor(t);
}
