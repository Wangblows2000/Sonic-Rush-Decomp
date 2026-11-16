#include <nitro/types.h>

typedef struct UnkStruct_0230be84_o {
    void *res[7];
    void *res2[4];
    void *unk2C;
} UnkStruct_0230be84_o;

typedef struct UnkStruct_0230be84 {
    u8 pad0[0x1d0];
    void *heap;
    u8 *mem;
} UnkStruct_0230be84;

extern u8 gGameLanguage;
extern UnkStruct_0230be84 *gSpecialStageWork;
extern char **gSpecialStageBgFileTable[];
extern char **gSpecialStageSpriteFileTable[];
extern void *data_ov001_0231df98;
extern void *gMainHeap;
extern u8 data_ov001_0231f5c8[];
extern char data_ov001_0231e6b4[];
extern u8 data_ov001_0231e6cc[];
extern void *gSoundHeap;

void *File_Load(void *p, s32 v);
void *func_ov001_0230b154(void *p);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void *NNS_SndHeapCreate(void *p, u32 size);
void func_0201d868(void *s, const char *path, void *a, u32 b);
void NNS_SndArcPlayerSetup(void *h);
void NNS_SndArcLoadSeqArc(u32 a, void *b);
void NNS_SndArcLoadBank(u32 a, void *b);
void func_02036854(void *a);
void NNS_SndArcLoadGroup(s32 a, void *b);

static inline u32 GetLang(void) {
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return gGameLanguage;
    default:
        return 1;
    }
}

void SpecialStage_LoadHudAssets(UnkStruct_0230be84_o *o) {
    UnkStruct_0230be84 *g = gSpecialStageWork;
    s32 i;
    char **a = gSpecialStageBgFileTable[GetLang()];
    char **b = gSpecialStageSpriteFileTable[GetLang()];
    u8 *blk;

    for (i = 0; i < 7; i++)
        o->res[i] = File_Load(a[i], -1);
    for (i = 0; i < 4; i++)
        o->res2[i] = File_Load(b[i], -1);
    o->unk2C = func_ov001_0230b154(data_ov001_0231df98);
    blk = Heap_Alloc(gMainHeap, 0x69040, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x69040);
        *(u32 *)blk = 0x69040;
        *(u32 *)(blk + 0x6903c) = 0x02467531;
        DC_StoreRange(blk, 0x69040);
        blk += 0x20;
    }
    g->mem = blk;
    MI_CpuFill8(g->mem, 0, 0x69000);
    g->heap = NNS_SndHeapCreate(g->mem, 0x69000);
    func_0201d868(data_ov001_0231f5c8, data_ov001_0231e6b4, g->heap, 0);
    NNS_SndArcPlayerSetup(g->heap);
    NNS_SndArcLoadSeqArc(0xe, g->heap);
    NNS_SndArcLoadBank(0x16, g->heap);
    NNS_SndArcLoadSeqArc(9, g->heap);
    NNS_SndArcLoadBank(0x11, g->heap);
    func_02036854(data_ov001_0231e6cc);
    NNS_SndArcLoadGroup(0x14, gSoundHeap);
}
