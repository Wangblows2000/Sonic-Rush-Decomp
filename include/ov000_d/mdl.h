#ifndef OV000_D_MDL_H
#define OV000_D_MDL_H

#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_Mdl;

typedef struct {
    s32 m[9];
} UnkMtx33_Mdl;

/* render object (passed to func_02031a68 / func_020315e0) */
typedef struct {
    u8 pad0[0x18];
    UnkVec_Mdl scale;
    UnkMtx33_Mdl rot;
    UnkVec_Mdl trans;
    u8 pad54[0x94 - 0x54];
    void *unk94;
} UnkStruct_MdlRender;

/* 0x160-byte model slot */
typedef struct {
    s32 cur;
    s32 prev;
    s32 frame;
    s32 angOfs;
    s32 flag;
    UnkStruct_MdlRender r;
    u8 padAC[0x120 - 0xAC];
    u8 unk120;
    u8 pad121[0x14C - 0x121];
    void *mdl14C;
    void *anm150;
    void *anm154;
    void *anm158;
    void *anm15C;
} UnkStruct_MdlSlot;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 scale;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 mode;
    s32 unk2C;
    s32 sel;
    s32 prevSel;
    UnkStruct_MdlSlot slot[2];
    void *unk2F8;
    void *unk2FC;
    void *unk300;
} UnkStruct_Mdl;

extern s16 data_0206e470[];

#define MDL_SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define MDL_COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void MTX_RotY33_(UnkMtx33_Mdl *m, s32 s, s32 c);
void func_02000e60(UnkMtx33_Mdl *m, s32 s, s32 c);
void func_02000e98(UnkMtx33_Mdl *m, s32 s, s32 c);
void func_02000f34(UnkMtx33_Mdl *a, UnkMtx33_Mdl *b, UnkMtx33_Mdl *out);
void func_02031a68(UnkStruct_MdlRender *r, s32 idx, void *anm, s32 frame, void *e);
void func_020315e0(UnkStruct_MdlRender *r);
void func_02025268(s32 cmd, void *data, s32 n);

#endif
