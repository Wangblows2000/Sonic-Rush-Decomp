#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2CC - 0x20C];
} UnkStruct_0230fa74;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 pad1D[3];
    s32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern s16 data_ov000_0236a05c;
extern s32 data_ov000_0236a060;
extern UnkStruct_022c4560 gGameState;
extern void *data_ov000_0236a06c;
extern u8 data_ov000_02366c58[], data_ov000_0236a064[], data_ov000_02366c78[];
extern u8 data_ov000_02364424[], data_ov000_0236a084[], data_ov000_0236a074[], data_ov000_0236a094[];
extern u8 data_ov000_0236a0a4[], data_ov000_023661c0[], data_ov000_023661a4[];

void GmkLand_Destructor(void);
void PaletteAnim_Create(s32 a, void *b, void *c, s32 d);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void GmkLand_Unk_ov000_0230f0bc(UnkStruct_0230fa74 *w);
void GmkLand_Action_Unk_ov000_0230f9cc(UnkStruct_0230fa74 *w);
void GmkLand_Action_Unk_ov000_0230f874(UnkStruct_0230fa74 *w);
void GmkLand_Action_Unk_ov000_0230f614(UnkStruct_0230fa74 *w);
void GmkLand_Action_Unk_ov000_0230f4fc(UnkStruct_0230fa74 *w);

UnkStruct_0230fa74 *GmkLand_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230fa74 *w;
    vu16 zero;
    u16 k;

    task = Task_Create(StageObj_Main, GmkLand_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_0230fa74));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230fa74));
    MapObject_Init(w, (s32)p, b, c);
    if (w->base.unk1E0->unk2 == 0xaf) {
        if (data_ov000_0236a05c == 0) {
            if (data_ov000_0236a060 == 0)
                data_ov000_0236a060 = gGameState.unk20;
        }
        data_ov000_0236a05c++;
    }
    if (p->unk2 == 0x76)
        StageObj_Load2dBAC(w, data_ov000_02366c58, 0, data_ov000_0236a064, data_ov000_02369ba0);
    else
        StageObj_Load2dBAC(w, data_ov000_02366c78, 0, &data_ov000_0236a06c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 5);
    if (gGameState.unk1C == 3 && p->unk2 != 0x76) {
        PaletteAnim_Create(w->base.spr0.unk44, data_ov000_02364424, data_ov000_0236a06c, 0);
        w->base.unk54 |= 0x800;
    }
    switch (p->unk2) {
    case 0x51:
    default:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a084);
        break;
    case 0x52:
        StageObj_SetSharedSpriteVram(w, 0x18, data_ov000_0236a074);
        break;
    case 0x53:
        StageObj_SetSharedSpriteVram(w, 0x20, data_ov000_0236a094);
        break;
    case 0x76:
        StageObj_SetSharedSpriteVram(w, 0x20, data_ov000_0236a0a4);
        break;
    }
    StageObj_SetOAMOrder(w, 0x14);
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    if (!(w->base.unk1E0->unk4 & 0x80) && w->base.unk1E0->unk2 != 0x53)
        w->base.unk1D0 = data_ov000_023661a4;
    k = w->base.unk1E0->unk2;
    if (k == 0x52) {
        w->base.unk1D8 = 0xc;
        w->base.unk1DA = 3;
        w->base.unk1D4 = -0x2f;
        w->base.unk1D6 = -0xb;
    } else if (k == 0x53) {
        w->base.unk1D8 = 6;
        w->base.unk1DA = 6;
        w->base.unk1D4 = -0x17;
        w->base.unk1D6 = -0x17;
    } else {
        w->base.unk1D8 = 6;
        w->base.unk1DA = 3;
        w->base.unk1D4 = -0x17;
        w->base.unk1D6 = -0xb;
        if (w->base.unk1D0 != NULL) {
            w->base.unk1DA = 1;
            w->base.unk1D6 += 4;
        }
    }
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
    switch (w->base.unk1E0->unk2) {
    default:
        GmkLand_Unk_ov000_0230f0bc(w);
        GmkLand_Action_Unk_ov000_0230f9cc(w);
        break;
    case 0x76:
        GmkLand_Action_Unk_ov000_0230f874(w);
        break;
    case 0xaf:
        GmkLand_Action_Unk_ov000_0230f614(w);
        w->base.unk4A += 0x100;
        break;
    case 0xef:
        GmkLand_Action_Unk_ov000_0230f4fc(w);
        w->base.unk4A += 0x200;
        break;
    }
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5C |= 2;
        w->base.unk1D6 -= 2;
    }
    w->base.unk1DC = Sound_AllocHandle();
    return w;
}
