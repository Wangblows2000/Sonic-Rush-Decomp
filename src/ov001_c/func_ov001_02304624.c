#include <nitro/types.h>

typedef struct UnkStruct_02304624_o {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_02304624_o;

typedef struct UnkStruct_02304624_e {
    u8 pad[0x2c];
} UnkStruct_02304624_e;

typedef struct UnkStruct_02304624 {
    UnkStruct_02304624_o obj0;
    UnkStruct_02304624_o obj1;
    void (*unk90)(void);
    u8 pad94[0xbc - 0x94];
    UnkStruct_02304624_e *ptrs[100];
    UnkStruct_02304624_e ents[100];
    u8 pad135C[0x1384 - (0x24c + 100 * 0x2c)];
    void *unk1384;
} UnkStruct_02304624;

typedef struct UnkStruct_02304624_h {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_02304624_h;

typedef struct { u8 pad[0x12]; u16 unk12; } E14_02304624;
typedef struct { u8 hdr[0xc]; E14_02304624 e[1]; } T14_02304624;

extern void *gMainHeap;
extern UnkStruct_02304624 *data_ov001_0231f548;
extern UnkStruct_02304624_h *data_ov001_0231f544;
extern u8 data_ov001_02316cac[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_ov001_02303934(void);
u32 ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i);
void AnimatorSprite_ProcessAnimation(void *c, u32 a, u32 b);

static inline u16 GetEnt(UnkStruct_02304624_h *h, u16 idx) {
    T14_02304624 *t = (T14_02304624 *)((u8 *)h + h->unk18);
    return t->e[idx].unk12;
}

void func_ov001_02304624(void) {
    u8 *blk;
    s32 i;
    UnkStruct_02304624_e *e;
    u32 r;

    blk = Heap_Alloc(gMainHeap, 0x13c0, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x13c0);
        *(u32 *)blk = 0x13c0;
        *(u32 *)(blk + 0x13bc) = 0x02467531;
        DC_StoreRange(blk, 0x13c0);
        blk += 0x20;
    }
    {
        vu16 zero = 0;
        data_ov001_0231f548 = (UnkStruct_02304624 *)blk;
        MIi_CpuClear16(zero, blk, 0x1388);
    }
    for (i = 0, e = data_ov001_0231f548->ents; i < 100; i++, e++)
        data_ov001_0231f548->ptrs[i] = e;
    data_ov001_0231f548->unk90 = func_ov001_02303934;
    r = ObjVram_Alloc(0, GetEnt(data_ov001_0231f544, 0));
    AnimatorSprite_Init(&data_ov001_0231f548->obj0, data_ov001_0231f544, 0, 0x4060, 0, 0, r, 0, 0x5000200);
    data_ov001_0231f548->obj0.unk44 = 0;
    data_ov001_0231f548->obj0.flags |= 4;
    AnimatorSprite_ProcessAnimation(&data_ov001_0231f548->obj0, 0, 0);
    r = ObjVram_Alloc(0, GetEnt(data_ov001_0231f544, 1));
    AnimatorSprite_Init(&data_ov001_0231f548->obj1, data_ov001_0231f544, 1, 0x4030, 0, 0, r, 0, 0x5000200);
    data_ov001_0231f548->obj1.unk44 = 0;
    data_ov001_0231f548->obj1.flags |= 4;
    AnimatorSprite_ProcessAnimation(&data_ov001_0231f548->obj1, 0, 0);
    data_ov001_0231f548->unk1384 = data_ov001_02316cac;
}
