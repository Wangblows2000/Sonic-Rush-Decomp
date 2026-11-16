#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02330aec_Vec;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_02330aec_Vec unk18;
} UnkStruct_02330aec_Sub;


typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x128 - 0x8C];
    UnkStruct_02330aec_Sub *unk128;
    u8 pad12C[0x134 - 0x12C];
    UnkStruct_Blk blk[9];
    u8 pad764[6];
    u16 unk76A;
} UnkStruct_02330aec;

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 unk10;
    s32 unk14;
} UnkStruct_02330aec_Arg;

extern void *gActComArchive;
extern u8 data_ov000_0236a9a0[];
extern u8 data_ov000_023682cc[];
extern u8 data_ov000_023682e4[];
extern u8 data_ov000_023682fc[];
extern u8 data_ov000_02368314[];
extern u8 data_ov000_0236832c[];
extern u8 data_ov000_02368344[];
extern u8 data_ov000_0236835c[];
extern u8 data_ov000_02368374[];
extern u8 data_ov000_0236838c[];

BOOL func_ov000_023270d8(UnkStruct_02330aec_Arg *a);
void StageObj_Destructor(void);
void func_ov000_02330848(void);

static inline void SetVec(UnkStruct_02330aec_Vec *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

UnkStruct_02330aec *Magnet_Create(UnkStruct_02330aec_Arg *arg) {
    UnkStruct_Task *task;
    UnkStruct_02330aec *w;
    vu16 zero;

    if (func_ov000_023270d8(arg))
        return NULL;
    if (arg->unk0 != 1)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02330aec));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02330aec));
    StageObj_LoadModelSimple(w, &w->blk[0], data_ov000_023682cc, 0, 1, 4, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[1], data_ov000_023682e4, 0, 2, 5, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[2], data_ov000_023682fc, 0, 3, 6, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[3], data_ov000_02368314, 0, 4, 7, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[4], data_ov000_0236832c, 0, 5, 2, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[5], data_ov000_02368344, 0, 6, 1, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[6], data_ov000_0236835c, 0, 0, 3, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[7], data_ov000_02368374, 0, 7, 0, data_ov000_0236a9a0, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[8], data_ov000_0236838c, 0, 8, 8, data_ov000_0236a9a0, gActComArchive);
    w->unk76A = 0x11;
    w->blk[6].unkA8 |= 0x40;
    w->blk[7].unkA8 |= 0x40;
    w->blk[8].unkA8 |= 0x40;
    SetVec(&w->unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_02330848;
    return w;
}
