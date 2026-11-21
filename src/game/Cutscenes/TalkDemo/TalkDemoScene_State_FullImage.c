#include <nitro/types.h>

typedef struct { s32 a; s32 b; } UnkPair_0206243c;
typedef struct { u8 pad0[8]; UnkPair_0206243c sub; u8 pad10[0x20]; s32 unk30; } UnkInfo_0206243c;
typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_0206243c;
typedef struct UnkWork_0206243c {
    void (*fn)(struct UnkWork_0206243c *w);
    u8 pad4[0x64];
    s32 unk68;
    UnkInfo_0206243c *info;  /* 0x6c */
    u8 pad70[4];
    s32 unk74;
    u8 pad78[6];
    u16 unk7E;
    u16 unk80;
} UnkWork_0206243c;
typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    s32 unk28;
    u16 unk2C;
    u16 unk2E;
    u16 unk30;
    u8 pad32[0xe];
} UnkParam_0206243c;

typedef struct UnkNode_0206243c UnkNode_0206243c;
extern UnkNode_0206243c *gPaletteTransferHead;
extern UnkNode_0206243c *gPaletteTransferTail;

UnkInfo_0206243c *TalkDemo_GetScriptPage(s32 a, u8 b);
UnkRes_0206243c *TalkDemo_LoadImageByPair(u8 a, u8 b);
s32 TalkDemo_GetCompressionType(void *p);
u32 TalkDemo_GetUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 TalkDemo_GetFlags(s32 a);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void PaletteTransfer_QueueLoad(void *p, u32 a, u32 b);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void TalkDemo_CreateFreeResTask(UnkRes_0206243c *res);
void TalkDemo_ApplyBg2Affine(UnkParam_0206243c *p);
void TalkDemoScene_State_FullImageWait(UnkWork_0206243c *w);

#pragma thumb on
void TalkDemoScene_State_FullImage(UnkWork_0206243c *w) {
    UnkInfo_0206243c *info;
    UnkRes_0206243c *res;
    u8 *p;

    w->info = TalkDemo_GetScriptPage(w->unk68, w->unk74);
    info = w->info;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x4404;
    {
        u8 buf[0x30] = {0};
    }
    { UnkPair_0206243c *q = &info->sub; res = TalkDemo_LoadImageByPair(q->a, q->b); }
    p = (u8 *)res + res->chr;
    switch (TalkDemo_GetCompressionType(p)) {
    case 0x10:
        MI_UncompressLZ16(p, (void *)0x06004000);
        break;
    case 0x20:
        MI_UncompressHuffman(p, (void *)0x06004000);
        break;
    case 0x30:
        MI_UncompressRL16(p, (void *)0x06004000);
        break;
    default:
        Mem_Copy(Compress_GetData(p), (void *)0x06004000, TalkDemo_GetUncompressedSize(p));
        break;
    }
    DC_StoreRange((void *)0x06004000, TalkDemo_GetUncompressedSize(p));
    p = (u8 *)res + res->pal;
    if (!(TalkDemo_GetFlags(0) & 1)) {
        UnkNode_0206243c *s0 = gPaletteTransferHead;
        UnkNode_0206243c *s1 = gPaletteTransferTail;
        gPaletteTransferHead = NULL;
        gPaletteTransferTail = NULL;
        PaletteTransfer_QueueCopy(p + 4, 0xf0, 0, 0x05000000);
        PaletteTransfer_Flush();
        gPaletteTransferHead = s0;
        gPaletteTransferTail = s1;
    } else {
        UnkNode_0206243c *s0 = gPaletteTransferHead;
        UnkNode_0206243c *s1 = gPaletteTransferTail;
        gPaletteTransferHead = NULL;
        gPaletteTransferTail = NULL;
        PaletteTransfer_QueueLoad(p, 1, 0x4000);
        PaletteTransfer_Flush();
        gPaletteTransferHead = s0;
        gPaletteTransferTail = s1;
    }
    ScreenTransfer_QueueRect((u8 *)res + res->scr, 0, 0, 0x20, (res->flags & 0x1000) ? 1 : 0, 9, 0, 4, 0, 0, 0x20, 7);
    TalkDemo_CreateFreeResTask(res);
    {
        UnkParam_0206243c prm = {0};
        prm.unk2C = 0x100;
        prm.unk2E = 0x100;
        prm.unk30 = 0;
        prm.unk24 = 0;
        prm.unk28 = 0x4400;
        TalkDemo_ApplyBg2Affine(&prm);
    }
    w->unk7E = 0;
    w->unk80 = info->unk30;
    w->fn = TalkDemoScene_State_FullImageWait;
}
#pragma thumb reset
