#ifndef OV000_C_STAGEOBJ_H
#define OV000_C_STAGEOBJ_H

#include <nitro/types.h>

/* 0x48-byte sprite/animation block */
typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
    u16 unk8;
    u16 unkA;
    u8 padC[0xE - 0xC];
    s16 unkE;
    void *unk10;
    u8 pad14[0x2A - 0x14];
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
    u32 unk20;
    u32 unk24;
    u8 unk28;
    u8 pad29[3];
} UnkStruct_Coll;

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u16 unk4;
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
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
    u8 unk6;
    u8 unk7;
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
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    u8 pad44[0x48 - 0x44];
    s16 unk48;
    s16 unk4A;
    u8 pad4C[2];
    s16 unk4E;
    u8 pad50[4];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u16 unk60;
    u16 unk62;
    s8 unk64;
    s8 unk65;
    s8 unk66;
    s8 unk67;
    void *unk68;
    u8 pad6C[0x70 - 0x6C];
    void *unk70;
    u8 pad74[4];
    void *unk78;
    u8 pad7C[0x80 - 0x7C];
    void *unk80;
    u8 pad84[0x88 - 0x84];
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
    UnkStruct_Coll coll2;
    void *unk1B8;
    void *unk1BC;
    u8 pad1C0[0x1C4 - 0x1C0];
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
    u8 unk1E5;
    u8 unk1E6;
    u8 unk1E7;
    s32 unk1E8;
    s32 unk1EC;
    u16 unk1F0;
    u8 pad1F2[2];
    u32 unk1F4;
    s16 unk1F8;
    u8 pad1FA[2];
    s32 unk1FC;
    s32 unk200;
    void *unk204;
    u8 pad208[4];
} UnkStruct_StageObj;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_Task;

void StageObj_Main(void);
UnkStruct_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
UnkStruct_Task *OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MapObject_Init(void *w, s32 a, s32 b, s32 c);
void StageObj_Load2dBAC(void *w, void *a, s32 b, void *c, void *d);
void StageObj_AllocSpritePalette(void *w, s32 a, s32 b);
void StageObj_SetOAMOrder(void *w, s32 a);
void StageObj_SetOAMPriority(void *w, s32 a);
void ObjAction_Load2dBAC(UnkStruct_Spr *a, UnkStruct_Spr *b, void *c, s32 d, void *e, void *f);
void AnimatorSprite_SetAnimation(UnkStruct_Spr *a, s32 b);
void SpritePair_SetOAMOrder(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c);
void func_02069e68(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c, void *d);
void SpritePair_SetOAMPriority(UnkStruct_Spr *a, UnkStruct_Spr *b, s32 c);
void ObjRect_SetBox2D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e);
void ObjRect_SetAtkDefFlags(UnkStruct_Coll *c, s32 a, s32 b);
void StageObj_SetAnimation(void *w, s32 a);
void func_02068e40(void *w, s32 a, s32 b, s32 c, s32 d);
void StageObj_SetSharedSpriteVram(void *w, s32 a, void *b);
s32 Sound_AllocHandle(void);

/* 0xB0-byte block */
typedef struct {
    u8 pad0[0xA8];
    u32 unkA8;
    u8 padAC[4];
} UnkStruct_Blk;

void StageObj_LoadModelSimple(void *w, UnkStruct_Blk *blk, void *tbl, s32 a, s32 b, s32 c, void *d, void *e);

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

extern u32 gRandSeed;

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void StageObj_Destructor(void);

#endif
