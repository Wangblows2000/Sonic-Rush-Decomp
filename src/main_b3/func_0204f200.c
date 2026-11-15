#include <nitro/types.h>

typedef struct { u8 unk0[0x28]; void *unk28; u8 pad2C[4]; u32 unk30; u8 pad34[4]; s32 unk38; u8 pad3C[4]; u32 unk40; } UnkStruct_0204f200;
typedef struct { u8 unk0[0x2c]; u32 unk2C; u8 pad30[0x14]; u16 unk44; } UnkSpr_0204f200;
typedef struct { u8 unk0[0x18]; s32 unk18; } UnkHdr_0204f200;
typedef struct { u16 unk0; u16 unk2; u16 unk4; u16 unk6; u8 pad8[0xc]; } UnkEnt_0204f200;
typedef struct { u8 pad0[0x1c]; UnkEnt_0204f200 ent[1]; } UnkTbl_0204f200;

void *func_02039c18(void *a, u32 b);
void func_0203363c(UnkSpr_0204f200 *s, UnkHdr_0204f200 *h, u32 idx, u32 c, void *d, u32 e, void *f, u32 g, u32 hh);
void func_0203344c(UnkSpr_0204f200 *s, u32 a, u32 b);

#pragma thumb on
static inline UnkEnt_0204f200 *GetEnt_0204f200(UnkHdr_0204f200 *h, u32 idx) { UnkTbl_0204f200 *t = (UnkTbl_0204f200 *)((u8 *)h + h->unk18); return &t->ent[idx]; }
void func_0204f200(UnkStruct_0204f200 *a, UnkSpr_0204f200 *spr, UnkHdr_0204f200 *h, u32 idx, u16 ofs) {
    u32 v;
    u32 dst;
    void *p;
    switch (a->unk38) {
    case 0x10:
        v = GetEnt_0204f200(h, idx)->unk0;
        break;
    case 0x100010:
        v = GetEnt_0204f200(h, idx)->unk2;
        break;
    case 0x200010:
        v = GetEnt_0204f200(h, idx)->unk4;
        break;
    case 0x300010:
        v = GetEnt_0204f200(h, idx)->unk6;
        break;
    default:
        return;
    }
    if (*(u32 *)&a->unk28 == 0)
        dst = 0x05000200;
    else
        dst = 0x05000600;
    func_0203363c(spr, h, idx, 0, a->unk28, 0, func_02039c18(a->unk28, v), 0, dst);
    spr->unk2C &= ~0x60000;
    spr->unk2C |= a->unk30 << 17;
    spr->unk44 = a->unk40 + ofs;
    func_0203344c(spr, 0, 0);
}
#pragma thumb reset
