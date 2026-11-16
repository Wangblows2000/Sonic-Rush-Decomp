#include <nitro/types.h>

typedef struct UnkStruct_022ee594_n {
    u8 pad0[0x18];
    struct UnkStruct_022ee594_n *next;
    struct UnkStruct_022ee594_n *prev;
} UnkStruct_022ee594_n;

typedef struct UnkStruct_022ee594_b {
    s32 unk0;
    UnkStruct_022ee594_n nodes[0x15];
    UnkStruct_022ee594_n *head;
    u8 pad2A8[4];
    s32 unk2AC;
    u16 unk2B0;
    u16 unk2B2;
    s32 unk2B4;
    s32 unk2B8;
    s32 unk2BC;
    s32 unk2C0;
    s32 unk2C4;
} UnkStruct_022ee594_b;

typedef struct UnkStruct_022ee594_p {
    u8 pad0[0x30];
    s32 unk30;
    u8 pad34[0x74 - 0x34];
    void (*unk74)(void);
    void (*unk78)(void);
    s32 unk7C;
    u8 pad80[0x1dc - 0x80];
    UnkStruct_022ee594_b *unk1DC;
} UnkStruct_022ee594_p;

typedef struct UnkStruct_022ee594_g {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_022ee594_g;

extern void *data_0208fe80;

UnkStruct_022ee594_p *func_ov001_023014f4(s32 a, s32 b);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
UnkStruct_022ee594_g *func_ov001_022ffce8(void);
void func_ov001_022ee100(UnkStruct_022ee594_n *out, UnkStruct_022ee594_p *p);
void func_ov001_022ee4c4(void);
void func_ov001_022ede0c(void);

void func_ov001_022ee594(s32 arg) {
    vu16 zero;
    vu32 fill;
    UnkStruct_022ee594_p *p;
    UnkStruct_022ee594_b *b;
    u8 *blk;
    s32 i;

    p = func_ov001_023014f4(1, 0x2f80);
    blk = func_02034a40(data_0208fe80, 0x300, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x300);
        *(u32 *)blk = 0x300;
        *(u32 *)(blk + 0x2fc) = 0x02467531;
        DC_StoreRange(blk, 0x300);
        blk += 0x20;
    }
    b = (UnkStruct_022ee594_b *)blk;
    zero = 0;
    MIi_CpuClear16(zero, b, 0x2c8);
    p->unk1DC = b;
    p->unk7C = arg;
    for (i = 0; i < 0x14; i++)
        b->nodes[i].next = &b->nodes[i + 1];
    b->nodes[0x14].next = &b->nodes[0];
    for (i = 0x14; i > 0; i--)
        b->nodes[i].prev = &b->nodes[i - 1];
    b->nodes[0].prev = &b->nodes[0x14];
    b->head = &b->nodes[0];
    b->unk0 = 0xc000;
    b->unk2AC = 0x14;
    if (func_ov001_022ffce8()->unk3C == 0) {
        b->unk2B0 = 0x3ff;
        b->unk2B2 = 0x17ff;
    } else {
        b->unk2B0 = 0x14bf;
        b->unk2B2 = 0x14bf;
    }
    b->unk2B4 = 5;
    b->unk2B8 = 0x12;
    b->unk2BC = 1;
    b->unk2C0 = 0x14;
    b->unk2C4 = 5;
    func_ov001_022ee100(&b->nodes[0], p);
    p->unk74 = func_ov001_022ee4c4;
    p->unk78 = func_ov001_022ede0c;
    p->unk30 = 0x2000;
}
