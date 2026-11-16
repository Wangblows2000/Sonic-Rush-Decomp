#include <nitro/types.h>

typedef struct UnkStruct_02300378 {
    u32 flags;
    u8 pad4[4];
    void *task8;
    void *taskC;
    void *task10;
    u8 pad14[0x3c - 0x14];
    s32 unk3C;
    u8 pad40[0x4c - 0x40];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 pad62[0x33c - 0x62];
    u32 unk33C;
} UnkStruct_02300378;

typedef struct UnkStruct_02300378_4560 {
    s32 unk0;
    u8 pad4[0x1c - 4];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    s32 unk20;
} UnkStruct_02300378_4560;

typedef struct UnkStruct_02300378_4590 {
    u8 pad0[0x14];
    u8 unk14;
    u8 pad15[3];
    s32 unk18;
} UnkStruct_02300378_4590;

extern void *gMainHeap;
extern UnkStruct_02300378 *gExtraBossWork;
extern UnkStruct_02300378_4560 gGameState;
extern UnkStruct_02300378_4590 data_022c4590;
extern u8 data_02074960[];
extern u8 data_020748bc[];

void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void StoryProgress_RestoreFromSave(void);
void NNS_SndHandleInit(u32 *p);
void ExtraBoss_InitPadMap(void);
void ExtraBoss_InitDisplay(void);
void ExtraBoss_LoadAssets(void);
void func_ov001_022f7308(void);
void func_ov001_0230582c(void);
void ExtraBossPlayerManager_Create(void);
void func_ov001_023030e0(void);
void func_ov001_02305474(void);
void func_ov001_02304624(void);
void func_ov001_022fdbc0(void);
void func_ov001_022fd6a4(void);
void func_ov001_02306738(void);
void NNS_G3dGlbLightColor(u32 index, u32 value);
void NNS_G3dGlbLightVector(u32 idx, s32 x, s32 y, s32 z);
void ExtraBoss_Main(void);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void ExtraBoss_LoadPlayerModel(s32 idx);
void ExtraBoss_ResetRound(void);
void func_ov001_022eec20(void);

void ExtraBoss_Start(void) {
    u8 *blk;
    u16 col;

    blk = Heap_Alloc(gMainHeap, 0x380, 0x20);
    if (OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x380);
        *(u32 *)blk = 0x380;
        *(u32 *)(blk + 0x37c) = 0x02467531;
        DC_StoreRange(blk, 0x380);
        blk += 0x20;
    }
    {
        vu16 zero = 0;
        gExtraBossWork = (UnkStruct_02300378 *)blk;
        MIi_CpuClear16(zero, blk, 0x340);
    }
    gExtraBossWork->flags |= 4;
    gExtraBossWork->unk4C = 0x32;
    gGameState.unk0 = 0;
    StoryProgress_RestoreFromSave();
    gExtraBossWork->unk60 = data_022c4590.unk14;
    gGameState.unk0 = 1;
    StoryProgress_RestoreFromSave();
    gExtraBossWork->unk61 = data_022c4590.unk14;
    NNS_SndHandleInit(&gExtraBossWork->unk33C);
    gGameState.unk1C = 8;
    gGameState.unk1D = 2;
    ExtraBoss_InitPadMap();
    ExtraBoss_InitDisplay();
    ExtraBoss_LoadAssets();
    func_ov001_022f7308();
    func_ov001_0230582c();
    ExtraBossPlayerManager_Create();
    func_ov001_023030e0();
    func_ov001_02305474();
    func_ov001_02304624();
    func_ov001_022fdbc0();
    func_ov001_022fd6a4();
    func_ov001_02306738();
    col = *(u16 *)(data_02074960 + 0x9c);
    NNS_G3dGlbLightColor(0, col);
    NNS_G3dGlbLightColor(1, col);
    NNS_G3dGlbLightColor(2, col);
    NNS_G3dGlbLightVector(0, *(s16 *)(data_020748bc + 0x90), *(s16 *)(data_020748bc + 0x92), *(s16 *)(data_020748bc + 0x94));
    NNS_G3dGlbLightVector(1, *(s16 *)(data_020748bc + 0x96), *(s16 *)(data_020748bc + 0x98), *(s16 *)(data_020748bc + 0x9a));
    NNS_G3dGlbLightVector(2, *(s16 *)(data_020748bc + 0x9c), *(s16 *)(data_020748bc + 0x9e), *(s16 *)(data_020748bc + 0xa0));
    gExtraBossWork->unk50 = 0x50000;
    gExtraBossWork->unk54 = 0;
    gExtraBossWork->unk58 = 0x100000;
    gExtraBossWork->unk5C = 0x130000;
    gExtraBossWork->task8 = Task_Create(ExtraBoss_Main, 0, 0x10, 0x1000, 0, 0);
    gExtraBossWork->taskC = Task_Create(0, 0, 0, 0xe000, 0, 0);
    gExtraBossWork->task10 = Task_Create(0, 0, 0, 0xe010, 0, 0);
    ExtraBoss_LoadPlayerModel(0);
    gExtraBossWork->unk3C = 0;
    ExtraBoss_ResetRound();
    func_ov001_022eec20();
    data_022c4590.unk18 = 0;
    gGameState.unk20 = 1;
}
