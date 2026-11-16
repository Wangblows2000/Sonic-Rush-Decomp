#include <nitro/types.h>

typedef struct UnkStruct_022f3530_a {
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
} UnkStruct_022f3530_a;

typedef struct UnkStruct_022f3530_s {
    u8 pad0[4];
    void (*unk4)(void);
    u32 unk8;
    u8 padC[0x38 - 0xc];
} UnkStruct_022f3530_s;

typedef struct UnkStruct_022f3530_o {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x78 - 0x24];
    void (*unk78)(void);
    void *unk7C;
    u8 pad80[4];
    UnkStruct_022f3530_a *unk84;
    u8 pad88[0x9c - 0x88];
    UnkStruct_022f3530_s *unk9C;
    UnkStruct_022f3530_s *unkA0;
    UnkStruct_022f3530_s *unkA4;
    UnkStruct_022f3530_s *unkA8;
    u8 padAC[4];
    UnkStruct_022f3530_s subB0;
    UnkStruct_022f3530_s subE8;
    UnkStruct_022f3530_s sub120;
    UnkStruct_022f3530_s sub158;
    u8 pad190[0x1dc - 0x190];
    void *unk1DC;
} UnkStruct_022f3530_o;

typedef struct UnkStruct_022f3530_d {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[4];
    UnkStruct_022f3530_o *unk18;
} UnkStruct_022f3530_d;

typedef struct UnkStruct_022f3530_p {
    u8 pad0[0x1c];
    s32 unk1C;
} UnkStruct_022f3530_p;

extern void *gMainHeap;
extern UnkStruct_022f3530_o *data_ov001_0231f078[];
extern void *data_ov001_0231f070;
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern u16 data_ov001_02316840[];

s32 ExtraBoss_GetWork(void);
UnkStruct_022f3530_o *func_ov001_023014f4(s32 a, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void func_ov001_022ee784(void);
void func_ov001_022ee73c(void);
void func_ov001_022f3164(void);
void func_ov001_022f2f14(void);
void func_ov001_022eee68(void);
void func_ov001_022eed0c(void);
void func_ov001_022eed64(void);
UnkStruct_022f3530_p *func_ov001_02300c94(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_022f2b78(UnkStruct_022f3530_o *o);

UnkStruct_022f3530_o *func_ov001_022f3530(void) {
    vu16 zero;
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    UnkStruct_022f3530_o *o;
    UnkStruct_022f3530_d *d;
    UnkStruct_022f3530_a *a;
    UnkStruct_022f3530_a *a2;
    UnkStruct_022f3530_o *o2;

    ExtraBoss_GetWork();
    o = func_ov001_023014f4(2, 0x2f00);
    data_ov001_0231f078[0] = o;
    d = (UnkStruct_022f3530_d *)Heap_Alloc(gMainHeap, 0x200, 0x20);
    if ((void *)func_02006abc(0) != d) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, d, 0x200);
        *(u32 *)d = 0x200;
        *(u32 *)((u8 *)d + 0x1fc) = 0x02467531;
        DC_StoreRange(d, 0x200);
        d = (UnkStruct_022f3530_d *)((u8 *)d + 0x20);
    }
    zero = 0;
    MIi_CpuClear16(zero, d, 0x1c4);
    o->unk1DC = d;
    a = (UnkStruct_022f3530_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)func_02006abc(0) != a) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, a, 0x160);
        *(u32 *)a = 0x160;
        *(u32 *)((u8 *)a + 0x15c) = 0x02467531;
        DC_StoreRange(a, 0x160);
        a = (UnkStruct_022f3530_a *)((u8 *)a + 0x20);
    }
    Model_Init(a, 0);
    Model_SetResource(a, data_ov001_0231f070, 2);
    a->unk18 = a->unk1C = a->unk20 = 0x34cc;
    o->unk84 = a;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[1], 0);
    a->unk10C |= 4;
    a->unk110 |= 4;
    a->unk114 = 0x1000;
    o->unk14 |= 0x40000;
    func_02021c38(a->unk90, func_ov001_022ee784, 0, 6, 3);
    a->unkA = 4;
    o->unk78 = func_ov001_022f3164;
    o->unk18 = 0x1c0000;
    o->unk1C = func_ov001_02300c94()->unk1C;
    if (o->unk1C > 0x111000)
        o->unk1C = 0xc0000;
    else if (o->unk1C < 0x65000)
        o->unk1C = 0x65000;
    o->unk20 = 0;
    func_ov001_022fe8d4(&o->subB0, o, 4, 0, 0xd000, 0xd000, 0x1e000, -0x14000, 0x5000, 0, 0);
    o->subB0.unk4 = func_ov001_022eee68;
    o->subB0.unk8 |= 0x3000;
    o->unk9C = &o->subB0;
    func_ov001_022fe8d4(&o->subE8, o, 1, 1, 0x1e000, 0x32000, 0x20000, 0, 0, 0, 0);
    o->subE8.unk8 |= 0x3000;
    o->unkA0 = &o->subE8;
    func_ov001_022fe8d4(&o->sub120, o, 1, 1, 0x32000, 0x1e000, 0x20000, 0, 0, 0, 0);
    o->sub120.unk8 |= 0x3200;
    o->unkA4 = &o->sub120;
    func_ov001_022fe8d4(&o->sub158, o, 1, 1, 0x1e000, 0x32000, 0x20000, 0, 0, 0, 0);
    o->sub158.unk4 = func_ov001_022eed0c;
    o->sub158.unk8 |= 0x3250;
    o->unkA8 = &o->sub158;

    o2 = func_ov001_023014f4(2, 0x2f01);
    data_ov001_0231f078[1] = o2;
    a2 = (UnkStruct_022f3530_a *)Heap_Alloc(gMainHeap, 0x160, 0x20);
    if ((void *)func_02006abc(0) != a2) {
        fill3 = 0xcccccccc;
        MIi_CpuClear32(fill3, a2, 0x160);
        *(u32 *)a2 = 0x160;
        *(u32 *)((u8 *)a2 + 0x15c) = 0x02467531;
        DC_StoreRange(a2, 0x160);
        a2 = (UnkStruct_022f3530_a *)((u8 *)a2 + 0x20);
    }
    Model_Init(a2, 0);
    Model_SetResource(a2, data_ov001_0231f070, 3);
    a2->unk18 = a2->unk1C = a2->unk20 = 0x34cc;
    o2->unk84 = a2;
    Model_SetAnimation(a2, 0, data_ov001_0231f068, data_ov001_02316840[2], 0);
    Model_SetAnimation(a2, 4, data_ov001_0231f074, data_ov001_02316840[3], 0);
    a2->unk10C |= 4;
    a2->unk110 |= 4;
    a2->unk114 = 0x1000;
    func_02021c38(a2->unk90, func_ov001_022ee73c, 0, 6, 3);
    a2->unkA = 4;
    o2->unk78 = func_ov001_022f2f14;
    o2->unk7C = data_ov001_0231f078[0];
    d->unk18 = o2;
    o2->unk14 |= 0x40000;
    o2->unk18 = o2->unk1C = o2->unk20 = 0;
    func_ov001_022fe8d4(&o2->subE8, o2, 1, 1, 0x32000, 0x1e000, 0x20000, 0, 0, 0, 0);
    o2->subE8.unk8 |= 0x3200;
    o2->unkA0 = &o2->subE8;
    func_ov001_022fe8d4(&o2->sub120, o2, 1, 1, 0xf000, 0xf000, 0x20000, 0, 0, 0, 0);
    o2->sub120.unk4 = func_ov001_022eed64;
    o2->sub120.unk8 |= 0x3220;
    o2->unkA4 = &o2->sub120;
    func_ov001_022f2b78(data_ov001_0231f078[0]);
    d->unk8 = -1;
    d->unk4 = 4;
    d->unkC = d->unk10 = 2;
    return o2;
}
