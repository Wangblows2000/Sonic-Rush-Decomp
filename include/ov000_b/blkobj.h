#ifndef OV000_B_BLKOBJ_H
#define OV000_B_BLKOBJ_H

#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_BlkVec;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_BlkVec unk18;
} UnkStruct_BlkObjSub;


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
    UnkStruct_BlkObjSub *unk128;
    u8 pad12C[0x134 - 0x12C];
    UnkStruct_Blk blk[9];
    u8 pad764[6];
    u16 unk76A;
} UnkStruct_BlkObj;

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 unk10;
    s32 unk14;
} UnkStruct_BlkObjArg;

extern void *data_022c448c;

BOOL func_ov000_023270d8(UnkStruct_BlkObjArg *a);
void func_0206bef4(void);

static inline void BlkObj_SetVec(UnkStruct_BlkVec *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

#endif
