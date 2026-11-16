#include <nitro/types.h>

typedef struct UnkStruct_02313cb4 {
    void *unk0;
    u8 anim[0x10c];
    u8 unk110;
    u8 pad111[0x13c - 0x111];
    void *unk13C;
    void *unk140;
    void *unk144;
} UnkStruct_02313cb4;

typedef struct UnkStruct_02313cb4_arc {
    u8 pad[0x5c];
} UnkStruct_02313cb4_arc;

extern void *gMainHeap;
extern UnkStruct_02313cb4 *data_ov001_0231f694;
extern u8 data_ov001_0231eef4[];
extern char data_ov001_0231ef0c[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
u8 *File_Load(void *p, s32 v);
u32 func_ov001_02314020(u32 *p);
u32 func_ov001_02314014(u32 *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
u32 *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
void *func_02021264(void *arc, u32 idx);
BOOL func_020212e0(void *arc);
void NNS_G3dResDefaultSetup(void *p);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void *NNS_G3dGetAnmByIdx(void *a, s32 b);
void NNS_G3dGlbPolygonAttr(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
void NNSi_G3dModifyPolygonAttrMask(void *p, s32 a, s32 b);

#define G data_ov001_0231f694

void SpecialStage_LoadPlayerArchive(void) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    UnkStruct_02313cb4_arc arc;
    u8 *blk;
    u8 *res;
    u8 *dst;
    u32 size;
    void *m;

    blk = Heap_Alloc(gMainHeap, 0x180, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, 0x180);
        *(u32 *)blk = 0x180;
        *(u32 *)(blk + 0x17c) = 0x02467531;
        DC_StoreRange(blk, 0x180);
        blk += 0x20;
    }
    G = (UnkStruct_02313cb4 *)blk;
    MI_CpuFill8(blk, 0, 0x148);
    res = File_Load(data_ov001_0231eef4, -1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    G->unk0 = blk;
    dst = G->unk0;
    switch (func_ov001_02314020((u32 *)res)) {
    case 0x10:
        MI_UncompressLZ16(res, dst);
        break;
    case 0x20:
        MI_UncompressHuffman(res, dst);
        break;
    case 0x30:
        MI_UncompressRL16(res, dst);
        break;
    default:
        Mem_Copy(Compress_GetData(res), dst, func_ov001_02314014((u32 *)res));
        break;
    }
    DC_StoreRange(dst, func_ov001_02314014((u32 *)res));
    if ((u8 *)OS_GetArenaLo(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill3 = 0xcccccccc;
            MIi_CpuClear32(fill3, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);
    func_02021310(&arc, data_ov001_0231ef0c, G->unk0);
    NNS_G3dResDefaultSetup(func_02021264(&arc, 0));
    G->unk13C = func_02021264(&arc, 0);
    G->unk140 = func_02021264(&arc, 1);
    G->unk144 = func_02021264(&arc, 2);
    func_020212e0(&arc);
    Model_Init(G->anim, 0);
    G->unk110 |= 4;
    Model_SetResource(G->anim, G->unk13C, 0);
    Model_SetAnimation(G->anim, 0, G->unk140, 5, 0);
    Model_SetAnimation(G->anim, 4, G->unk144, 1, 0);
    m = NNS_G3dGetAnmByIdx(G->unk13C, 0);
    NNS_G3dGlbPolygonAttr(0, 0, 3, 0, 0x1f, 0x8000);
    NNSi_G3dModifyPolygonAttrMask(m, 0, 0xc0);
}
