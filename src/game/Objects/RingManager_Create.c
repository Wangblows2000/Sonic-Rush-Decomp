#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x2A];
    u16 unk2A;
    u32 unk2C;
} UnkStruct_0235c238_ResHdr;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_0235c238_Res;

typedef struct {
    u8 pad0[4];
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 unkA4[0xA];
    u8 unkAE;
    u8 unkAF;
    u8 padB0[0xBC - 0xB0];
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    u8 padC8[0x160 - 0xC8];
    u32 unk160;
    u8 pad164[0x198 - 0x164];
    s32 unk198;
    s32 unk19C;
    u8 pad1A0[0x9D6 - 0x1A0];
    u8 unk9D6;
    u8 pad9D7;
} UnkStruct_0235c238;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_Task *gRingManagerTask;
extern void *gZoneActArchive;
extern void *gActComArchive;
extern u8 data_ov000_023690e8[], data_ov000_023690ec[], data_ov000_02369100[];

void RingManager_Main(void);
void RingManager_Destructor(void);
void RingManager_BoxCheck(void);
void RingManager_BoxCheckArena1(void);
void RingManager_BoxCheckArena2(void);
void RingManager_BoxCheckArena3(void);
void RingManager_CollideMapAll(void);
void RingManager_CollideMap(void);
void RingManager_DrawRingArena3(void);
void RingManager_SpawnEffectArena3(void);
void RingManager_DrawRingArena2(void);
void RingManager_SpawnEffectArena2(void);
void RingManager_DrawRingArena1(void);
void RingManager_SpawnEffectArena1(void);
void RingManager_DrawRing3D(void);
void RingManager_SpawnEffect3D(void);
void RingManager_DrawRing2D(void);
void RingManager_SpawnEffect2D(void);

void NNS_FndMountArchive(void *f, void *name, void *heap);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 idx);
void NNS_FndUnmountArchive(void *f);
s32 VramTex_Alloc(u32 a, s32 b);
s32 VramPltt_Alloc(u16 a, s32 b);
void Model_InitSprite3D(void *s, void *res, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Model_Animate(void *s);
void *ObjData_Load(s32 a, void *name, void *heap);
void ObjDraw_LoadPaletteRow(void *a, s32 b, s32 c, s32 d);
s32 ObjVram_Alloc(s32 a, s32 b);
void AnimatorSprite_Init(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, u32 g);

static inline BOOL func_ov000_0235c238_IsNormal(void) {
    BOOL r = FALSE;
    if (gGameState.unk1C < 9 && gGameState.unk1D < 2)
        r = TRUE;
    return r;
}

UnkStruct_0235c238 *RingManager_Create(void) {
    UnkStruct_Task *task;
    UnkStruct_0235c238 *w;
    u32 file[0x5c / 4];
    vu16 zero;
    UnkStruct_0235c238_Res *res;
    UnkStruct_0235c238_ResHdr *hdr;
    s32 a;
    void *mdl;

    task = Task_Create(RingManager_Main, RingManager_Destructor, 0, 0x3000, 2, sizeof(UnkStruct_0235c238));
    gRingManagerTask = task;
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0235c238));
    w->unk9D6 = 1;
    if (gGameState.unk18 & 0x20)
        w->unk9D6 = 2;
    w->unkC = RingManager_BoxCheck;
    if (!func_ov000_0235c238_IsNormal()) {
        if (gGameState.unk1C == 1)
            w->unkC = RingManager_BoxCheckArena1;
        else if (gGameState.unk1C == 2)
            w->unkC = RingManager_BoxCheckArena2;
        else if (gGameState.unk1C == 3)
            w->unkC = RingManager_BoxCheckArena3;
    }
    if (!func_ov000_0235c238_IsNormal() && gGameState.unk1C == 7)
        w->unk10 = RingManager_CollideMapAll;
    else
        w->unk10 = RingManager_CollideMap;
    if (!func_ov000_0235c238_IsNormal()) {
        if (gGameState.unk1C == 3) {
            w->unk4 = RingManager_DrawRingArena3;
            w->unk8 = RingManager_SpawnEffectArena3;
        } else if (gGameState.unk1C == 2) {
            w->unk4 = RingManager_DrawRingArena2;
            w->unk8 = RingManager_SpawnEffectArena2;
        } else if (gGameState.unk1C == 1) {
            w->unk4 = RingManager_DrawRingArena1;
            w->unk8 = RingManager_SpawnEffectArena1;
        } else {
            w->unk4 = RingManager_DrawRing3D;
            w->unk8 = RingManager_SpawnEffect3D;
        }
        NNS_FndMountArchive(file, data_ov000_023690e8, gZoneActArchive);
        res = NNS_FndGetArchiveFileByIndex(file, 0);
        NNS_FndUnmountArchive(file);
        hdr = (UnkStruct_0235c238_ResHdr *)((u8 *)res + res->unk18);
        a = VramTex_Alloc(hdr->unk2C, 0);
        Model_InitSprite3D(w->unkA4, res, 0, 0, a, VramPltt_Alloc(hdr->unk2A, 0), 0, 0);
        w->unkAE = 0x14;
        w->unkAF = 7;
        w->unkC4 = 0x800;
        w->unkC0 = w->unkC4;
        w->unkBC = w->unkC0;
        Model_Animate(w->unkA4);
        w->unk160 |= 0x10;
        mdl = ObjData_Load(0, data_ov000_023690ec, gActComArchive);
        ObjDraw_LoadPaletteRow(mdl, 0, 2, 0);
        ObjDraw_LoadPaletteRow(mdl, 0, 2, 1);
    } else {
        w->unk4 = RingManager_DrawRing2D;
        w->unk8 = RingManager_SpawnEffect2D;
        mdl = ObjData_Load(0, data_ov000_02369100, gActComArchive);
        AnimatorSprite_Init(&w->spr0, mdl, 0, 0, 0, 0, ObjVram_Alloc(0, 2), 0, 0x5000200);
        AnimatorSprite_Init(&w->spr1, mdl, 0, 0, 1, 0, ObjVram_Alloc(1, 2), 0, 0x5000600);
        w->unk198 = ObjVram_Alloc(0, 2);
        w->unk19C = ObjVram_Alloc(1, 2);
        w->spr0.unk44 = 2;
        w->spr1.unk44 = 2;
        w->spr0.unk2C |= 0x4010;
        w->spr1.unk2C |= 0x4010;
        w->spr0.unk46 = 8;
        w->spr1.unk46 = 8;
        SpritePair_SetOAMPriority(&w->spr0, &w->spr1, 2);
        ObjDraw_LoadPaletteRow(mdl, 0, 2, 0);
        ObjDraw_LoadPaletteRow(mdl, 0, 2, 1);
    }
    return w;
}
