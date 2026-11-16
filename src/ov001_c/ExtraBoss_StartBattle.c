#include <nitro/types.h>

typedef struct UnkStruct_022ffeb0_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022ffeb0_t;

typedef struct UnkStruct_022ffeb0 {
    u32 flags;
    s32 unk4;
    UnkStruct_022ffeb0_t *task8;
    UnkStruct_022ffeb0_t *taskC;
    UnkStruct_022ffeb0_t *task10;
    u8 pad14[0x40 - 0x14];
    s32 unk40;
    u8 pad44[0x50 - 0x44];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
} UnkStruct_022ffeb0;

typedef struct UnkStruct_022ffeb0_g {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022ffeb0_g;

extern UnkStruct_022ffeb0 *gExtraBossWork;
extern u8 data_02074ba4[];

UnkStruct_022ffeb0_g *ExtraBoss_GetWork(void);
void NNS_SndPlayerStopSeq(void *pp, s32 a);
s32 FX_ModS32(s32 n, s32 d);
void ExtraBoss_State_Battle(void);
void func_ov001_022fef3c(void);
void func_ov001_022feee4(void);
void func_ov001_02302a3c(void);
void func_ov001_02304fb4(void);
void func_ov001_02304460(void);
void func_ov001_022fd4d4(void);
void func_ov001_0230582c(void);
void func_ov001_023057f4(void);
void func_ov001_022ee964(void);
void func_ov001_02306424(void);
void G3X_SetEdgeColorTable(const void *src);

static inline BOOL IsMulti_022ffeb0(void) {
    if (ExtraBoss_GetWork()->unk40 > 1)
        return TRUE;
    return FALSE;
}

void ExtraBoss_StartBattle(void) {
    NNS_SndPlayerStopSeq((u8 *)ExtraBoss_GetWork() + 0x33c, 0);
    if (FX_ModS32(gExtraBossWork->unk40, 2) != 0)
        *(vu16 *)0x4000304 &= ~0x8000;
    else
        *(vu16 *)0x4000304 |= 0x8000;
    gExtraBossWork->task8->fn = ExtraBoss_State_Battle;
    gExtraBossWork->taskC->fn = func_ov001_022fef3c;
    gExtraBossWork->task10->fn = func_ov001_022feee4;
    gExtraBossWork->flags &= ~0x72;
    gExtraBossWork->flags |= 4;
    func_ov001_02302a3c();
    func_ov001_02304fb4();
    func_ov001_02304460();
    func_ov001_022fd4d4();
    if (IsMulti_022ffeb0()) {
        gExtraBossWork->unk50 = 0;
        gExtraBossWork->unk54 = 0;
        gExtraBossWork->unk58 = 0x180000;
        gExtraBossWork->unk5C = 0x180000;
    } else {
        gExtraBossWork->unk50 = 0x50000;
        gExtraBossWork->unk54 = 0;
        gExtraBossWork->unk58 = 0x100000;
        gExtraBossWork->unk5C = 0x130000;
    }
    func_ov001_0230582c();
    func_ov001_023057f4();
    if (IsMulti_022ffeb0())
        func_ov001_022ee964();
    *(vu16 *)0x4000008 = *(vu16 *)0x4000008 & ~3;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1300;
    func_ov001_02306424();
    G3X_SetEdgeColorTable(data_02074ba4);
}
