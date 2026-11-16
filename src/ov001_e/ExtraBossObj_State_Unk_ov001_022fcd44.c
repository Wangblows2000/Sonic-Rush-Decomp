#include <nitro/types.h>

typedef struct UnkStruct_022fcd44_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 unk10D;
    u8 unk10E;
    u8 unk10F;
} UnkStruct_022fcd44_o;

typedef struct UnkStruct_022fcd44_w {
    u8 pad0[4];
    s32 type;
} UnkStruct_022fcd44_w;

typedef struct UnkStruct_022fcd44_p {
    u8 pad0[0x1dc];
    UnkStruct_022fcd44_w *work;
} UnkStruct_022fcd44_p;

typedef struct UnkStruct_022fcd44 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x74 - 0x18];
    void (*state)(struct UnkStruct_022fcd44 *p);
    u8 pad78[4];
    UnkStruct_022fcd44_p *parent;
    u8 pad80[4];
    UnkStruct_022fcd44_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
} UnkStruct_022fcd44;

typedef struct UnkStruct_022fcd44_c {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_022fcd44_c;

UnkStruct_022fcd44_c *ExtraBoss_GetWork(void);
void ExtraBoss_SwapObjModel(UnkStruct_022fcd44 *p, s32 n);
void ExtraBoss_CopyParentPosition(UnkStruct_022fcd44 *p);
void ExtraBossObj_State_Unk_ov001_022fce7c(UnkStruct_022fcd44 *p);
void ExtraBossObj_State_Unk_ov001_022fcbd4(UnkStruct_022fcd44 *p);
void ExtraBossObj_State_Unk_ov001_022fd000(UnkStruct_022fcd44 *p);

void ExtraBossObj_State_Unk_ov001_022fcd44(UnkStruct_022fcd44 *p) {
    UnkStruct_022fcd44_w *w = p->parent->work;

    if (p->unk84->unk10C & 2) {
        p->state = ExtraBossObj_State_Unk_ov001_022fd000;
        ExtraBoss_SwapObjModel(p, 0x17);
        p->unk84->unk10C |= 4;
        p->unk84->unk10D |= 4;
    }
    switch (w->type) {
    case 0:
    case 1:
    case 2:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        break;
    case 3:
    case 4:
        p->state = ExtraBossObj_State_Unk_ov001_022fce7c;
        p->unk1CC = 0;
        ExtraBoss_SwapObjModel(p, 0x18);
        p->unk84->unk10C &= ~4;
        p->unk84->unk10D &= ~4;
        p->unk84->unk10F &= ~4;
        ExtraBoss_CopyParentPosition(p);
        return;
    case 5:
    case 6:
    case 12:
        p->state = ExtraBossObj_State_Unk_ov001_022fcbd4;
        p->flags |= 0x30000;
        return;
    }
    ExtraBoss_CopyParentPosition(p);
}
