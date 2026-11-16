#ifndef OV000_B_STAGEOBJ_H
#define OV000_B_STAGEOBJ_H

#include <nitro/types.h>

/* 0x48-byte sprite/animation block */
typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x2A - 8];
    s16 unk2A;
    u32 unk2C;
    u8 pad30[4];
    s32 unk34;
    u8 pad38[0x44 - 0x38];
    u16 unk44;
    u16 unk46;
} UnkStruct_Spr;

/* 0x2C-byte collision box */
typedef struct {
    u8 pad0[0xA];
    u16 unkA;
    u8 padC[0x18 - 0xC];
    void *unk18;
    void *unk1C;
    u8 pad20[0x28 - 0x20];
    u8 unk28;
    u8 pad29[3];
} UnkStruct_Coll;

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u16 unk4;
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
} UnkStruct_ObjParam;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_Vec;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_Vec unk18;
} UnkStruct_Obj128;

static inline void Obj_SetVec(UnkStruct_Vec *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

/* common 0x20C-byte stage object header */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x3C - 0x1C];
    s16 unk3C;
    s16 unk3E;
    u8 pad40[0x48 - 0x40];
    s16 unk48;
    s16 unk4A;
    u8 pad4C[2];
    s16 unk4E;
    u8 pad50[4];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x70 - 0x6C];
    void *unk70;
    u8 pad74[0x88 - 0x74];
    void *unk88;
    void **unk8C;
    u8 pad90[0x94 - 0x90];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    void *unk124;
    UnkStruct_Obj128 *unk128;
    u8 pad12C[0x134 - 0x12C];
    UnkStruct_Coll coll0;
    UnkStruct_Coll coll1;
    u8 pad18C[0x1B8 - 0x18C];
    void *unk1B8;
    u8 pad1BC[0x1C4 - 0x1BC];
    u16 unk1C4;
    u8 pad1C6[2];
    void *unk1C8;
    s32 unk1CC;
    void *unk1D0;
    s16 unk1D4;
    s16 unk1D6;
    u16 unk1D8;
    u16 unk1DA;
    s32 unk1DC;
    UnkStruct_ObjParam *unk1E0;
    u8 unk1E4;
    u8 pad1E5[3];
    s32 unk1E8;
    s32 unk1EC;
    u8 pad1F0[4];
    u32 unk1F4;
    s16 unk1F8;
    u8 pad1FA[0x204 - 0x1FA];
    s32 unk204;
    u8 pad208[4];
} UnkStruct_StageObj;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_Task;

void func_0206c3d0(void);
UnkStruct_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
UnkStruct_Task *func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov000_022fba50(void *w, s32 a, s32 b, s32 c);
void func_02069f1c(void *w, void *a, s32 b, void *c, void *d);
void func_0206a054(void *w, s32 a, s32 b);
void func_02069918(void *w, s32 a);
void func_020698f0(void *w, s32 a);
void func_02069d38(UnkStruct_Spr *a, UnkStruct_Spr *b, void *c, s32 d, void *e, void *f);
void func_02033550(UnkStruct_Spr *a, s32 b);
void func_0206990c(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c);
void func_02069e68(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c, void *d);
void func_020698bc(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c);
void func_0206e3c0(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e);
void func_0206e2b4(UnkStruct_Coll *c, s32 a, s32 b);
void func_02069818(void *w, s32 a);
void func_02068e40(void *w, s32 a, s32 b, s32 c, s32 d);
void func_02069eb0(void *w, s32 a, void *b);
s32 func_0203663c(void);

/* 0xB0-byte block */
typedef struct {
    u8 pad0[0xA8];
    u32 unkA8;
    u8 padAC[4];
} UnkStruct_Blk;

void func_02069a34(void *w, UnkStruct_Blk *blk, void *tbl, s32 a, s32 b, s32 c, void *d, void *e);

/* generic 0x134-byte effect object */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 pad2C[2];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[0x40 - 0x34];
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[4];
    s16 unk62;
    u8 pad64[4];
    void *unk68;
    u8 pad6C[4];
    void *unk70;
    u8 pad74[0x80 - 0x74];
    void *unk80;
    u8 pad84[4];
    void *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
} UnkStruct_EffObj;

extern u32 data_0208fe70;

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_0206bef4(void);

#endif
