#include <nitro/types.h>

typedef struct UnkStruct_022fcbd4_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 unk10D;
    u8 unk10E;
    u8 unk10F;
} UnkStruct_022fcbd4_o;

typedef struct UnkStruct_022fcbd4_s {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_022fcbd4_s;

typedef struct UnkStruct_022fcbd4_q {
    u8 pad0[0x1dc];
    UnkStruct_022fcbd4_s *unk1DC;
} UnkStruct_022fcbd4_q;

typedef struct UnkStruct_022fcbd4 {
    u8 pad0[0x14];
    u32 unk14;
    u8 pad18[0x74 - 0x18];
    void (*unk74)(void);
    u8 pad78[0x7c - 0x78];
    UnkStruct_022fcbd4_q *unk7C;
    u8 pad80[4];
    UnkStruct_022fcbd4_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
} UnkStruct_022fcbd4;

typedef struct UnkStruct_022fcbd4_g {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_022fcbd4_g;

UnkStruct_022fcbd4_g *ExtraBoss_GetWork(void);
void ExtraBoss_CopyParentPosition(UnkStruct_022fcbd4 *p);
void ExtraBoss_SwapObjModel(UnkStruct_022fcbd4 *p, s32 a);
void ExtraBossObj_State_Unk_ov001_022fce7c(void);
void ExtraBossObj_State_Unk_ov001_022fd000(void);

void ExtraBossObj_State_Unk_ov001_022fcbd4(UnkStruct_022fcbd4 *p) {
    UnkStruct_022fcbd4_s *s = p->unk7C->unk1DC;
    s32 a;
    s32 b;

    if (s->unk4 == 5)
        return;
    if (s->unk4 == 0xc)
        return;
    if (s->unk4 == 6)
        return;
    if (ExtraBoss_GetWork()->unk3C == 0) {
        a = 0x17;
        b = 0x18;
    } else {
        a = 0x19;
        b = 0x1a;
    }
    p->unk14 &= ~0x30000;
    if (s->unk4 == 3 || s->unk4 == 4) {
        p->unk74 = ExtraBossObj_State_Unk_ov001_022fce7c;
        p->unk1CC = 0;
        ExtraBoss_SwapObjModel(p, b);
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk84->unk10C &= ~4;
            p->unk84->unk10D &= ~4;
            p->unk84->unk10F &= ~4;
        } else {
            p->unk84->unk10C |= 4;
            p->unk84->unk10D |= 4;
            p->unk84->unk10F |= 4;
        }
    } else {
        p->unk74 = ExtraBossObj_State_Unk_ov001_022fd000;
        ExtraBoss_SwapObjModel(p, a);
        p->unk84->unk10C |= 4;
        p->unk84->unk10D |= 4;
        p->unk84->unk10F |= 4;
    }
    ExtraBoss_CopyParentPosition(p);
}
