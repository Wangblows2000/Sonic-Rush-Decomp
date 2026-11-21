#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[0x134 - 0x58];
    u8 unk134[0x24];
    u8 unk158[0x24];
} UnkStruct_022dfbc8;

extern s16 data_0206e470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *out);

void Boss6_State_PieceSpinFly(UnkStruct_022dfbc8 *p) {
    u8 mtx[0x24];
    u16 a;
    u8 *cam;

    a = 0;
    p->unkC++;
    p->unk10 += p->unk8;
    cam = p->unk134;
    p->unk14 -= 0x100;
    if (p->unk8 > 0)
        a = 0x8000;
    MTX_RotY33_(mtx, data_0206e470[((a & 0xffff) >> 4) * 2], data_0206e470[((a & 0xffff) >> 4) * 2 + 1]);
    MTX_RotZ33_(cam + 0x24, data_0206e470[(((u16)(p->unkC * 0x2aaa) & 0xffff) >> 4) * 2], data_0206e470[(((u16)(p->unkC * 0x2aaa) & 0xffff) >> 4) * 2 + 1]);
    MTX_Concat33(cam + 0x24, mtx, cam + 0x24);
    if (p->unkC >= 0x78)
        p->unk54 |= 4;
}
