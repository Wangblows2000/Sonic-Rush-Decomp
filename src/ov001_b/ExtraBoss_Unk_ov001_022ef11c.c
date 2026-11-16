#include <nitro/types.h>

typedef struct UnkStruct_022ef11c_a {
    u8 pad0[0xa];
    u8 unkA;
    u8 padB[0x18 - 0xb];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x90 - 0x24];
    u8 unk90[0x10c - 0x90];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    u8 pad118[0x12c - 0x118];
    u16 unk12C;
} UnkStruct_022ef11c_a;

typedef struct UnkStruct_022ef11c_o UnkStruct_022ef11c_o;

typedef struct UnkStruct_022ef11c_d {
    u8 pad0[0x14];
    u16 unk14;
    u16 unk16;
    UnkStruct_022ef11c_o *unk18;
} UnkStruct_022ef11c_d;

struct UnkStruct_022ef11c_o {
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
    u8 pad78[0x84 - 0x78];
    UnkStruct_022ef11c_a *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_022ef11c_d *unk1DC;
};

extern void *gMainHeap;
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f070;
extern void *data_ov001_0231f074;
extern u16 data_ov001_02316840[];

void ExtraBoss_StartCharacterSwitch(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, s32 e);
void NNS_G3dRenderObjResetCallBack(void *s);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void ExtraBossObj_State_Unk_ov001_022ef0a0(void);
void ExtraBossObj_State_Unk_ov001_022ef10c(void);
void ExtraBossObj_State_Unk_ov001_022eef90(void);
UnkStruct_022ef11c_o *ExtraBossObj_Create(s32 a, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);

void ExtraBoss_Unk_ov001_022ef11c(UnkStruct_022ef11c_o *w) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    UnkStruct_022ef11c_d *d;
    UnkStruct_022ef11c_o *o2;
    UnkStruct_022ef11c_a *a2;
    UnkStruct_022ef11c_a *a;
    UnkStruct_022ef11c_a *na;
    UnkStruct_022ef11c_o *o;
    UnkStruct_022ef11c_o *o1;
    UnkStruct_022ef11c_a *na1;
    UnkStruct_022ef11c_a *na3;
    UnkStruct_022ef11c_o *o3;

    d = w->unk1DC;
    a = w->unk84;
    o2 = d->unk18;
    a2 = o2->unk84;
    ExtraBoss_StartCharacterSwitch();
    BrightnessLerp_Create(0x20, 0x10, 0, 3, 0);
    NNS_G3dRenderObjResetCallBack(a2->unk90);
    d->unk14 = d->unk16 = 0;
    o2->unk14 |= 0x30000;
    w->unk18 = w->unk1C = w->unk20 = 0;
    w->unk50 = w->unk54 = w->unk58 = 0;
    w->unk5C = w->unk60 = w->unk64 = 0;
    a->unk10C |= 0x40;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0xc], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[0xd], 0);
    a->unk12C = 0x400;
    a->unk10C &= ~4;
    a->unk110 &= ~4;
    a->unk114 = 0x2000;
    a2->unk10C |= 0x40;
    Model_SetAnimation(a2, 0, data_ov001_0231f068, data_ov001_02316840[0xe], 0);
    Model_SetAnimation(a2, 4, data_ov001_0231f074, data_ov001_02316840[0xf], 0);
    a2->unk12C = 0x400;
    a2->unk10C &= ~4;
    a2->unk110 &= ~4;
    a2->unk114 = 0x2000;
    w->unk74 = ExtraBossObj_State_Unk_ov001_022ef0a0;
    o2->unk74 = ExtraBossObj_State_Unk_ov001_022ef10c;
    w->unk1C8 = 0xb4;

    o1 = ExtraBossObj_Create(2, 0x2f01);
    na1 = (UnkStruct_022ef11c_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != na1) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, na1, 0x160);
        *(u32 *)na1 = 0x160;
        *(u32 *)((u8 *)na1 + 0x15c) = 0x02467531;
        DC_StoreRange(na1, 0x160);
        na1 = (UnkStruct_022ef11c_a *)((u8 *)na1 + 0x20);
    }
    Model_Init(na1, 0);
    Model_SetResource(na1, data_ov001_0231f070, 1);
    na1->unk18 = na1->unk1C = na1->unk20 = 0x34cc;
    o1->unk84 = na1;
    o1->unk74 = ExtraBossObj_State_Unk_ov001_022ef10c;

    o = ExtraBossObj_Create(2, 0x2f01);
    na = (UnkStruct_022ef11c_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != na) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, na, 0x160);
        *(u32 *)na = 0x160;
        *(u32 *)((u8 *)na + 0x15c) = 0x02467531;
        DC_StoreRange(na, 0x160);
        na = (UnkStruct_022ef11c_a *)((u8 *)na + 0x20);
    }
    Model_Init(na, 0);
    Model_SetResource(na, data_ov001_0231f070, 4);
    na->unk18 = na->unk1C = na->unk20 = 0x34cc;
    o->unk84 = na;
    o->unk74 = ExtraBossObj_State_Unk_ov001_022ef10c;
    o->unk14 |= 0x30000;

    o3 = ExtraBossObj_Create(2, 0x2f01);
    na3 = (UnkStruct_022ef11c_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)OS_GetArenaLo(0) != na3) {
        fill3 = 0xcccccccc;
        MIi_CpuClear32(fill3, na3, 0x160);
        *(u32 *)na3 = 0x160;
        *(u32 *)((u8 *)na3 + 0x15c) = 0x02467531;
        DC_StoreRange(na3, 0x160);
        na3 = (UnkStruct_022ef11c_a *)((u8 *)na3 + 0x20);
    }
    Model_Init(na3, 0);
    Model_SetResource(na3, data_ov001_0231f070, 0);
    na3->unk18 = na3->unk1C = na3->unk20 = 0x34cc;
    o3->unk84 = na3;
    na3->unkA = 4;
    Model_SetAnimation(na3, 0, data_ov001_0231f068, 4, 0);
    na3->unk10C &= ~4;
    na3->unk114 = 0;
    o3->unk74 = ExtraBossObj_State_Unk_ov001_022eef90;
    o3->unk1C8 = 0x119;
}
