#include <nitro/types.h>

typedef struct UnkStruct_022f41c4_a {
    u8 pad0[0xa];
    u8 unkA;
    u8 padB[0x18 - 0xb];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x90 - 0x24];
    u8 unk90[0x10c - 0x90];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10d];
    s32 unk114;
} UnkStruct_022f41c4_a;

typedef struct UnkStruct_022f41c4_o UnkStruct_022f41c4_o;
struct UnkStruct_022f41c4_o {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x50 - 0x24];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    u8 pad68[0x74 - 0x68];
    void (*unk74)(void);
    void (*unk78)(void);
    UnkStruct_022f41c4_o *unk7C;
    u8 pad80[4];
    UnkStruct_022f41c4_a *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
};

extern void *gMainHeap;
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f070;

void ExtraBoss_StartCharacterSwitch(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_02021c24(void *s);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void func_ov001_022f4148(void);
void func_ov001_022f41b4(void);
void func_ov001_022ee784(void);
void func_ov001_022f4028(void);
void func_ov001_022f3d6c(void);
UnkStruct_022f41c4_o *func_ov001_023014f4(s32 a, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);

void func_ov001_022f41c4(UnkStruct_022f41c4_o *w) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    vu32 fill4;
    UnkStruct_022f41c4_a *a;
    UnkStruct_022f41c4_o *o;
    UnkStruct_022f41c4_o *o3;

    ExtraBoss_StartCharacterSwitch();
    BrightnessLerp_Create(0x20, 0x10, 0, 3, 0);
    func_02021c24(w->unk84->unk90);
    w->unk18 = w->unk1C = w->unk20 = 0;
    w->unk50 = w->unk54 = w->unk58 = 0;
    w->unk5C = w->unk60 = w->unk64 = 0;
    a = w->unk84;
    Model_SetAnimation(a, 0, data_ov001_0231f068, 3, 0);
    a->unk10C &= ~4;
    a->unk114 = 0x2000;
    w->unk74 = func_ov001_022f4148;
    w->unk1C8 = 0xb4;

    o = func_ov001_023014f4(2, 0x2f01);
    a = (UnkStruct_022f41c4_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_022f41c4_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f070, 1);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk84 = a;
    o->unk74 = func_ov001_022f41b4;
    o->unk14 |= 0x30000;

    o = func_ov001_023014f4(2, 0x2f01);
    a = (UnkStruct_022f41c4_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_022f41c4_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f070, 4);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk84 = a;
    o->unk74 = func_ov001_022f41b4;

    o3 = func_ov001_023014f4(2, 0x2f01);
    a = (UnkStruct_022f41c4_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill3 = 0xcccccccc;
        MIi_CpuClear32(fill3, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_022f41c4_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f070, 2);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o3->unk84 = a;
    a->unkA = 4;
    func_02021c38(a->unk90, func_ov001_022ee784, 0, 6, 3);
    Model_SetAnimation(a, 0, data_ov001_0231f068, 0x1c, 0);
    a->unk10C &= ~4;
    a->unk114 = 0;
    o3->unk74 = 0;
    o3->unk1C8 = 0x119;
    o3->unk14 |= 0x30000;

    o = func_ov001_023014f4(2, 0x2f01);
    a = (UnkStruct_022f41c4_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != a) {
        fill4 = 0xcccccccc;
        MIi_CpuClear32(fill4, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_022f41c4_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f070, 3);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk84 = a;
    a->unkA = 4;
    Model_SetAnimation(a, 0, data_ov001_0231f068, 0x35, 0);
    a->unk10C &= ~4;
    a->unk114 = 0;
    o->unk74 = func_ov001_022f4028;
    o->unk1C8 = 0x119;
    o->unk78 = func_ov001_022f3d6c;
    o->unk7C = o3;
}
