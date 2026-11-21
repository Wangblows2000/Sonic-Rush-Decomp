#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    void *res;
} UnkStruct_02357dbc_Src;

typedef struct {
    s32 unk0;
    UnkStruct_02357dbc_Src *a;
    UnkStruct_02357dbc_Src *b;
    u8 bufA[0x200];
    u8 bufB[0x200];
} UnkStruct_02357dbc;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_02357dbc_Task;

UnkStruct_02357dbc_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void PlayerSelectMenu_PaletteAnim_Main(void);
u32 PlayerSelectMenu_GetCompressionType(void *res);
u32 PlayerSelectMenu_GetCompressedSize(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void UnkDecomp_02357dbc(void *res, void *dst) {
    switch (PlayerSelectMenu_GetCompressionType(res)) {
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
        Mem_Copy(Compress_GetData(res), dst, PlayerSelectMenu_GetCompressedSize(res));
        break;
    }
    DC_StoreRange(dst, PlayerSelectMenu_GetCompressedSize(res));
}

#pragma thumb on

void PlayerSelectMenu_CreatePaletteAnim(UnkStruct_02357dbc_Src *a, UnkStruct_02357dbc_Src *b) {
    UnkStruct_02357dbc *w = Task_Create(PlayerSelectMenu_PaletteAnim_Main, NULL, 0, 0x2000, 0x1000, sizeof(UnkStruct_02357dbc))->work;

    w->unk0 = 0;
    w->a = a;
    w->b = b;
    UnkDecomp_02357dbc(a->res, w->bufA);
    UnkDecomp_02357dbc(b->res, w->bufB);
}

#pragma thumb reset
