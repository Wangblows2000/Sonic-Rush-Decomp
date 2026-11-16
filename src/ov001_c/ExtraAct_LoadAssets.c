#include <nitro/types.h>

typedef struct UnkStruct_022f9ab4_arc {
    u8 pad[0x60];
} UnkStruct_022f9ab4_arc;

typedef struct UnkStruct_022f9ab4 {
    u32 unk0;
    u32 unk4;
    u32 res[4];
} UnkStruct_022f9ab4;

extern void *gMainHeap;
extern u8 data_ov001_0231d5a0[];
extern char data_ov001_0231d5bc[];
extern u8 data_ov001_0231d5c0[];
extern u8 *data_ov001_0231f088;
extern UnkStruct_022f9ab4 *data_ov001_0231f084;
extern void *gSoundHeap;

u8 *File_Load(void *p, s32 v);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov001_022f9d14(u32 *p);
u32 func_ov001_022f9d08(u32 *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
u32 *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
u32 func_02021264(void *arc, u32 idx);
void NNS_G3dResDefaultSetup(u32 p);
BOOL func_020212e0(void *arc);
void func_02036854(void *a);
void NNS_SndArcLoadGroup(s32 a, void *b);

void ExtraAct_LoadAssets(void) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_022f9ab4_arc arc;
    u8 *res;
    u32 size;
    u8 *blk;
    s32 i;

    res = File_Load(data_ov001_0231d5a0, -1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    data_ov001_0231f088 = blk;
    switch (func_ov001_022f9d14((u32 *)res)) {
    case 0x10:
        MI_UncompressLZ16(res, blk);
        break;
    case 0x20:
        MI_UncompressHuffman(res, blk);
        break;
    case 0x30:
        MI_UncompressRL16(res, blk);
        break;
    default:
        Mem_Copy(Compress_GetData(res), blk, func_ov001_022f9d08((u32 *)res));
        break;
    }
    DC_StoreRange(blk, func_ov001_022f9d08((u32 *)res));
    if ((u8 *)OS_GetArenaLo(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);
    func_02021310(&arc, data_ov001_0231d5bc, data_ov001_0231f088);
    data_ov001_0231f084->unk0 = func_02021264(&arc, 6);
    data_ov001_0231f084->unk4 = func_02021264(&arc, 5);
    for (i = 0; i < 4; i++)
        data_ov001_0231f084->res[i] = func_02021264(&arc, i + 7);
    NNS_G3dResDefaultSetup(data_ov001_0231f084->res[1]);
    NNS_G3dResDefaultSetup(data_ov001_0231f084->res[0]);
    func_020212e0(&arc);
    func_02036854(data_ov001_0231d5c0);
    NNS_SndArcLoadGroup(0xa, gSoundHeap);
}
