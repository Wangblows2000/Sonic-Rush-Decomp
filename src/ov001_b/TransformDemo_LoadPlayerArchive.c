#include <nitro/types.h>

typedef struct UnkStruct_022de388_a {
    u8 pad0[0x10c];
    u8 unk10C[5];
    u8 pad111[0x138 - 0x111];
} UnkStruct_022de388_a;

typedef struct UnkStruct_022de388 {
    void *unk0;
    void *unk4[5];
    UnkStruct_022de388_a anims[6];
    void *unk768[6];
} UnkStruct_022de388;

typedef struct UnkStruct_022de388_c {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022de388_c;

typedef struct UnkStruct_022de388_t {
    s32 v[6];
} UnkStruct_022de388_t;

typedef struct UnkStruct_022de388_arc {
    u8 pad[0x5c];
} UnkStruct_022de388_arc;

extern void *gMainHeap;
extern u8 data_ov001_0231c3ac[];
extern char data_ov001_0231c3c8[];
extern UnkStruct_022de388_t data_ov001_02316244[];
extern void *gTransformDemoModelList[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u8 *File_Load(void *p, s32 v);
u32 TransformDemo_GetCompressionType(u32 *p);
u32 TransformDemo_GetUncompressedSize(u32 *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
u32 *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
BOOL NNS_FndMountArchive(void *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(void *arc, u32 idx);
BOOL NNS_FndUnmountArchive(void *arc);
void Model_Init(void *s, u32 a);
void NNS_G3dResDefaultSetup(void *p);
void Model_SetResource(void *s, void *res, u32 idx);
u32 NNS_G3dGetTex(void *h);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);

void TransformDemo_LoadPlayerArchive(UnkStruct_022de388_c *c, UnkStruct_022de388 *s) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_022de388_arc arc;
    u8 *res;
    u32 size;
    u8 *dst;
    u8 *blk;
    u8 i;
    u8 j;
    s32 *tbl;
    s32 e;
    UnkStruct_022de388_a *a;
    u32 r;

    res = File_Load(data_ov001_0231c3ac, -1);
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
    s->unk0 = blk;
    dst = s->unk0;
    switch (TransformDemo_GetCompressionType((u32 *)res)) {
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
        Mem_Copy(Compress_GetData(res), dst, TransformDemo_GetUncompressedSize((u32 *)res));
        break;
    }
    DC_StoreRange(dst, TransformDemo_GetUncompressedSize((u32 *)res));
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
    NNS_FndMountArchive(&arc, data_ov001_0231c3c8, s->unk0);
    for (i = 0; i < 5; i++)
        s->unk4[i] = NNS_FndGetArchiveFileByIndex(&arc, i);
    NNS_FndUnmountArchive(&arc);
    tbl = data_ov001_02316244[c->unk10].v;
    for (i = 0; i < 6; i++) {
        if (tbl[i] != -1)
            s->unk768[i] = File_Load(gTransformDemoModelList[tbl[i]], -1);
    }
    tbl = data_ov001_02316244[c->unk10].v;
    for (i = 0; i < 6; i++) {
        a = &s->anims[i];
        e = tbl[i];
        if (e == -1)
            continue;
        Model_Init(a, 0);
        NNS_G3dResDefaultSetup(s->unk768[i]);
        Model_SetResource(a, s->unk768[i], 0);
        r = NNS_G3dGetTex(s->unk768[i]);
        for (j = 0; j < 5; j++) {
            if (j == 2 || j == 3) {
                if (r != 0) {
                    Model_SetAnimation(a, j, s->unk4[j], e, r);
                    a->unk10C[j] |= 4;
                }
            } else {
                Model_SetAnimation(a, j, s->unk4[j], e, 0);
                a->unk10C[j] |= 4;
            }
        }
    }
    *(s32 *)((u8 *)s + 0x64) = 0x226000;
    *(s32 *)((u8 *)s + 0x19c) = -0x226000;
}
