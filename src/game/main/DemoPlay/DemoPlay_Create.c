#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u32 pad14;
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_02048ba4;

typedef struct {
    u8 pad0[0x1c];
    u32 unk1c;
    u32 unk20;
} UnkState2_02048ba4;

typedef struct {
    u8 pad0[0x48];
    u16 unk48[4];
    u8 pad50[0x24];
    u16 unk74;
    u16 unk76;
    u32 pad78;
} UnkDual_02048ba4;

typedef struct {
    u32 pad0;
    u16 id;
    u16 flags;
    void *res;
    UnkDual_02048ba4 sprA;
    UnkDual_02048ba4 sprB;
} UnkWork_02048ba4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048ba4 *work;
} UnkTask_02048ba4;

extern UnkState_02048ba4 gGameState;
extern UnkState2_02048ba4 data_022c4590;
extern u8 data_02073bd0[];
extern u8 data_02073bd1[];
extern u8 data_02073bd2[];
extern u8 data_02073bd4[];
extern u8 data_02073bd6[];
extern u8 gPad[];
extern u8 gGameLanguage;
extern char data_02073b38[][0x19];

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void Stage_ResetProgress(void);
void StageStart_Create(s32 a);
void DemoInput_Create(void *a, void *res);
void *File_Load(void *a, s32 b);
u32 ObjVram_Alloc(s32 a, u32 b);
void AnimatorSpriteDS_Init(UnkDual_02048ba4 *d, void *res, u16 idx, u32 mask, u32 flags, u32 a0, u32 a1, u32 b0, u32 b1, u32 c0, u32 c1, u32 e0, u32 e1);
void AnimatorSpriteDS_ProcessAnimation(UnkDual_02048ba4 *d, s32 a, s32 b);
void DemoPlay_Main(void);
void DemoPlay_Destructor(void);

#define ENT(sym, i, T) (*(T *)((sym) + (i) * 0x22))

void DemoPlay_Create(u16 id) {
    UnkWork_02048ba4 *w;
    u32 v;
    u32 palA;
    u32 palB;
    UnkDual_02048ba4 *d;

    w = ((UnkTask_02048ba4 *)Task_Create(DemoPlay_Main, DemoPlay_Destructor, 0, 0x4000, 3, 0x104))->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x104);
    }
    w->id = id;
    Stage_ResetProgress();
    gGameState.zone = ENT(data_02073bd0, w->id, u8);
    gGameState.act = ENT(data_02073bd1, w->id, u8);
    gGameState.unk0 = ENT(data_02073bd2, w->id, u16);
    gGameState.flags |= 4;
    if (ENT(data_02073bd4, w->id, u16) != 0 || ENT(data_02073bd6, w->id, u16) != 0) {
        gGameState.flags |= 8;
        data_022c4590.unk1c = ENT(data_02073bd4, w->id, u16);
        data_022c4590.unk20 = ENT(data_02073bd6, w->id, u16);
        w->flags |= 4;
    }
    StageStart_Create(3);
    gGameState.unk10 = 3;
    DemoInput_Create(gPad, &ENT(data_02073bd0, w->id, u8) + 0xa);
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        v = gGameLanguage;
        break;
    default:
        v = 1;
        break;
    }
    w->res = File_Load(data_02073b38[v], 0);

    d = &w->sprA;
    palA = ObjVram_Alloc(0, 0x18);
    palB = ObjVram_Alloc(1, 0x18);
    AnimatorSpriteDS_Init(d, w->res, 2, 0, 0x10, 0, palA, 0, 0x05000200, 0, palB, 0, 0x05000600);
    d->unk74 = 2;
    d->unk76 = 2;

    d = &w->sprB;
    palA = ObjVram_Alloc(0, 0x30);
    palB = ObjVram_Alloc(1, 0x30);
    AnimatorSpriteDS_Init(d, w->res, 0, 0, 0x10, 0, palA, 0, 0x05000200, 0, palB, 0, 0x05000600);
    d->unk74 = 2;
    d->unk76 = 2;

    w->sprA.unk48[0] = 0;
    w->sprA.unk48[1] = 0;
    w->sprA.unk48[2] = 0;
    w->sprA.unk48[3] = 0;
    w->sprB.unk48[0] = 0;
    w->sprB.unk48[1] = 0;
    w->sprB.unk48[2] = 0;
    w->sprB.unk48[3] = 0;
    AnimatorSpriteDS_ProcessAnimation(&w->sprA, 0, 0);
    AnimatorSpriteDS_ProcessAnimation(&w->sprB, 0, 0);
}
