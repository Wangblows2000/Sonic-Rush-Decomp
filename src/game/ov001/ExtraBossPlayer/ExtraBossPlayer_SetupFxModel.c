#include <nitro/types.h>

typedef struct UnkStruct_022fd2dc_d {
    u32 flags;
    u8 pad4[0x18];
} UnkStruct_022fd2dc_d;

typedef struct UnkStruct_022fd2dc_r {
    void *res;
    void *anims[5];
} UnkStruct_022fd2dc_r;

typedef struct UnkStruct_022fd2dc {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022fd2dc;

extern void *gMainHeap;
extern UnkStruct_022fd2dc_d gExtraBossFxResTable[];
extern UnkStruct_022fd2dc_r gExtraBossFxRes[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Model_Init(void *s, u32 a);
BOOL NNS_G3dResDefaultSetup(void *res);
void Model_SetResource(void *s, void *res, u32 idx);
u32 NNS_G3dGetTex(void *h);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);

UnkStruct_022fd2dc *ExtraBossPlayer_SetupFxModel(UnkStruct_022fd2dc *o, s32 n) {
    vu32 fill;
    s32 i;
    u32 tex;

    if (o == NULL) {
        u8 *blk = Heap_Alloc(gMainHeap, 0x160, 0x20);
        if ((u8 *)OS_GetArenaLo(0) != blk) {
            fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, 0x160);
            *(u32 *)blk = 0x160;
            *(u32 *)(blk + 0x15c) = 0x02467531;
            DC_StoreRange(blk, 0x160);
            blk += 0x20;
        }
        o = (UnkStruct_022fd2dc *)blk;
        Model_Init(o, 0);
    }
    if (!(gExtraBossFxResTable[n].flags & 1))
        NNS_G3dResDefaultSetup(gExtraBossFxRes[n].res);
    Model_SetResource(o, gExtraBossFxRes[n].res, 0);
    o->unk20 = 0x34cc;
    o->unk1C = o->unk20;
    o->unk18 = o->unk1C;
    tex = NNS_G3dGetTex(gExtraBossFxRes[n].res);
    for (i = 0; i < 5; i++) {
        if (gExtraBossFxRes[n].anims[i] != NULL)
            Model_SetAnimation(o, i, gExtraBossFxRes[n].anims[i], 0, tex);
    }
    return o;
}
