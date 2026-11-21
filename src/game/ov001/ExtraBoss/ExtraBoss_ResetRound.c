#include <nitro/types.h>

typedef struct UnkStruct_022fecb0_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022fecb0_t;

typedef struct UnkStruct_022fecb0 {
    u32 flags;
    s32 unk4;
    UnkStruct_022fecb0_t *task8;
    UnkStruct_022fecb0_t *taskC;
    UnkStruct_022fecb0_t *task10;
    u8 pad14[0x40 - 0x14];
    s32 unk40;
    s32 unk44;
    u8 pad48[4];
    s32 unk4C;
    u8 pad50[0x318 - 0x50];
    s32 unk318;
} UnkStruct_022fecb0;

extern UnkStruct_022fecb0 *gExtraBossWork;
extern s16 gDisplaySub[];
extern s16 gDisplayMain[];
extern u8 data_02074960[];
extern u8 data_020748bc[];
extern u8 data_02074ba4[];

void ExtraBoss_Main(void);
void ExtraBossPlayer_Action_LockControl(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);
void NNS_G3dGlbLightColor(u32 index, u32 value);
void NNS_G3dGlbLightVector(u32 idx, s32 x, s32 y, s32 z);
void G3X_SetEdgeColorTable(const void *src);
void StageTitleCard_CreateExtraBoss(void);
void Sound_PlaySeq(u32 *p, u32 a);

void ExtraBoss_ResetRound(void) {
    u16 col;

    *(vu16 *)0x4000304 |= 0x8000;
    gExtraBossWork->flags |= 2;
    gExtraBossWork->flags &= ~0x74;
    gExtraBossWork->task8->fn = ExtraBoss_Main;
    gExtraBossWork->taskC->fn = 0;
    gExtraBossWork->task10->fn = 0;
    ExtraBossPlayer_Action_LockControl();
    gExtraBossWork->unk4C = 0x32;
    gExtraBossWork->unk4 = 0;
    gExtraBossWork->unk44 = 0;
    gExtraBossWork->unk318 = 0;
    gDisplaySub[0x24] = -0x10;
    gDisplayMain[0x24] = gDisplaySub[0x24];
    GXx_SetMasterBrightness_((vu16 *)0x400006c, gDisplayMain[0x24]);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, gDisplaySub[0x24]);
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 2;
    *(vu16 *)0x400000c = *(vu16 *)0x400000c & ~3;
    col = *(u16 *)(data_02074960 + 0x9c);
    NNS_G3dGlbLightColor(0, col);
    NNS_G3dGlbLightColor(1, col);
    NNS_G3dGlbLightColor(2, col);
    NNS_G3dGlbLightVector(0, *(s16 *)(data_020748bc + 0x90), *(s16 *)(data_020748bc + 0x92), *(s16 *)(data_020748bc + 0x94));
    NNS_G3dGlbLightVector(1, *(s16 *)(data_020748bc + 0x96), *(s16 *)(data_020748bc + 0x98), *(s16 *)(data_020748bc + 0x9a));
    NNS_G3dGlbLightVector(2, *(s16 *)(data_020748bc + 0x9c), *(s16 *)(data_020748bc + 0x9e), *(s16 *)(data_020748bc + 0xa0));
    G3X_SetEdgeColorTable(data_02074ba4);
    StageTitleCard_CreateExtraBoss();
    if (gExtraBossWork->unk40 < 3)
        Sound_PlaySeq(0, 0x14);
    else
        Sound_PlaySeq(0, 0x15);
}
