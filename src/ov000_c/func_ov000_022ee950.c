#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[0x18 - 0xD];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x58 - 0x24];
    s32 unk58;
    u8 pad5C[0x138 - 0x5C];
} UnkStruct_022ee950_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022ee950_Sub unk20C;
    u8 unk344[4];
    void *unk348;
    u8 pad34C[0x358 - 0x34C];
    void *unk358;
    u8 pad35C[0x378 - 0x35C];
    UnkStruct_Coll unk378;
    UnkStruct_Coll unk3A4;
    s32 unk3D0;
    u8 pad3D4[4];
    u8 unk3D8[0x430 - 0x3D8];
    s16 unk430;
    s16 unk432;
    s16 unk434;
    s16 unk436;
    s16 unk438;
    s16 unk43A;
    void *unk43C;
    void *unk440;
    u8 pad444[0x452 - 0x444];
    u16 unk452;
    u8 pad454[0x4C0 - 0x454];
    void *unk4C0;
    void *unk4C4;
    void *unk4C8;
    void *unk4CC;
    void *unk4D0;
    void *unk4D4;
} UnkStruct_022ee950;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x214 - 0x1C];
    UnkStruct_022ee950 *unk214;
} UnkStruct_02369b78;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_02369b78 *data_ov000_02369b78;
extern UnkStruct_022c4560 data_022c4560;
extern void *gZoneActArchive;
extern void *data_ov000_0236ac28;
extern u8 data_ov000_02365aec[], data_ov000_02365af0[], data_ov000_0236ac58[], data_ov000_02365afc[];
extern u8 data_ov000_0236ac50[], data_ov000_02365b08[], data_ov000_0236ac40[], data_ov000_02365b14[];
extern u8 data_ov000_0236ac48[], data_ov000_02365b20[], data_ov000_0236ac38[];
extern s16 data_ov000_02368b7e[][0x18];
extern s16 data_ov000_02368ba4[][0x18];
extern s16 data_ov000_02368ba6[][0x18];

void func_ov000_022ebda0(void);
void func_ov000_022eb3c0(void);
void func_ov000_022ea8f8(void);
void func_ov000_022e98d8(void);
void func_ov000_022eb9f8(void);
void func_ov000_022eb970(void);
void func_ov000_022eb5fc(void);
void Pad_Reset(void *p, s32 a, s32 b);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void StageObj_LoadModel(void *w, void *s, void *a, s32 b, void *c, void *d);
void StageObj_LoadMotion(void *w, void *s, void *a, void *b, void *c);
void AnimatorSprite_Init(UnkStruct_Spr *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, u32 h);
void func_ov000_022ee198(UnkStruct_022ee950 *w, s32 a);

UnkStruct_022ee950 *func_ov000_022ee950(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_02369b78 *mgr = data_ov000_02369b78;
    UnkStruct_022ee950 *w;
    UnkStruct_Coll *coll;
    UnkStruct_022ee950_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, func_ov000_022ebda0, 0, 0x501, 2, sizeof(UnkStruct_022ee950))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ee950));
    mgr->unk214 = w;
    w->base.unk68 = func_ov000_022eb3c0;
    w->unk43C = func_ov000_022ea8f8;
    w->unk440 = func_ov000_022e98d8;
    w->unk452 = Rand() & 3;
    Pad_Reset(w->unk3D8, 0, 0);
    func_02021310(ctx, data_ov000_02365aec, gZoneActArchive);
    w->unk4CC = func_02021264(ctx, 8);
    w->unk4D0 = func_02021264(ctx, 9);
    func_020212e0(ctx);
    if (data_022c4560.unk0 == 0) {
        w->unk3D0 = 1;
        StageObj_LoadModel(w, &w->unk20C, data_ov000_02365af0, 0, data_ov000_0236ac58, data_ov000_0236ac28);
    } else {
        w->unk3D0 = 0;
        StageObj_LoadModel(w, &w->unk20C, data_ov000_02365afc, 0, data_ov000_0236ac50, data_ov000_0236ac28);
    }
    w->unk4C0 = w->unk344;
    StageObj_LoadMotion(w, &w->unk20C, data_ov000_02365b08, data_ov000_0236ac40, data_ov000_0236ac28);
    w->unk4C4 = w->unk348;
    StageObj_LoadMotion(w, &w->unk20C, data_ov000_02365b14, data_ov000_0236ac48, data_ov000_0236ac28);
    w->unk4C8 = w->unk358;
    StageObj_Load2dBAC(w, data_ov000_02365b20, 0, data_ov000_0236ac38, data_ov000_0236ac28);
    w->unk4D4 = w->base.spr0.unk10;
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022eb9f8;
    w->base.unk78 = func_ov000_022eb970;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x88080;
    w->base.unk5C |= 0x20;
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = mgr->unk18;
    w->unk430 = data_ov000_02368b7e[w->unk3D0][0] / 2;
    w->unk432 = w->unk430 * 0.15f;
    w->unk434 = w->unk430 * 0.3f;
    w->unk436 = w->unk430 * 0.5f;
    w->unk438 = w->unk430 * 0.7f;
    w->unk43A = w->unk430 * 0.9f;
    w->base.unk40 = data_ov000_02368ba4[w->unk3D0][0];
    w->base.unk42 = data_ov000_02368ba6[w->unk3D0][0];
    w->base.unk64 = -6;
    w->base.unk66 = 6;
    w->base.unk65 = -12;
    w->base.unk67 = 13;
    coll = &w->unk378;
    *coll = w->base.coll0;
    ObjRect_SetAtkDefFlags(coll, 0x11, 0);
    coll->unkA |= 0x40;
    coll->unk1C = func_ov000_022eb5fc;
    coll->unk18 = w;
    coll = &w->unk3A4;
    *coll = w->base.coll1;
    coll->unk18 = w;
    sub = &w->unk20C;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk58 = -0x10000;
    sub->unkA = 4;
    sub->unkB = 1;
    sub->unkC = 0xf;
    AnimatorSprite_Init(&w->base.spr0, w->unk4D4, 0, 0x18, 0, 0, 0, 0, 0);
    func_ov000_022ee198(w, 0x3c);
    w->base.unk5C |= 4;
    return w;
}
