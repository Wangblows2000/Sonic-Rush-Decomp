#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_Mdl;

typedef struct {
    UnkStruct_StageObj base;
    u16 unk20C;
    u16 unk20E;
    u8 pad210[0x228 - 0x210];
    UnkStruct_Mdl unk228;
} UnkStruct_022f80b4;

extern UnkStruct_Task *data_022b4574;
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020316f4(void *m);
void func_020315e0(void *m);

void func_ov000_022f80b4(void) {
    UnkStruct_022f80b4 *w = data_022b4574->work;
    UnkStruct_Mdl *m = &w->unk228;
    u32 mtx[9];
    s32 idx;

    if (w->base.unk5C & 0x20)
        return;
    m->unk48 = w->base.unk10 << 4;
    m->unk4C = -(w->base.unk14 << 4);
    m->unk50 = w->base.unk18 << 4;
    if (w->base.unk5C & 1)
        MTX_RotY33_(m->unk24, data_0206e470[0x1800], data_0206e470[0x1801]);
    else
        MTX_RotY33_(m->unk24, data_0206e470[0x800], data_0206e470[0x801]);
    idx = (s32)(w->unk20C & 0xffff) >> 4;
    func_02000e98(mtx, SIN_IDX(idx), COS_IDX(idx));
    func_02000f34(m->unk24, mtx, m->unk24);
    idx = (s32)(w->unk20E & 0xffff) >> 4;
    MTX_RotY33_(mtx, SIN_IDX(idx), COS_IDX(idx));
    func_02000f34(m->unk24, mtx, m->unk24);
    func_020316f4(m);
    func_020315e0(m);
}
