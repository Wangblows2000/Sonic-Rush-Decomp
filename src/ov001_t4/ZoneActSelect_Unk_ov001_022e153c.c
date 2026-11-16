#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[4];
    u16 w;
    u16 h;
} UnkRes_022e153c;

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022e153c;

typedef struct {
    u8 pad0[0xe0];
    u8 bg0[0x40];
    u8 bg1[0x40];
    u8 buf[0x200];
    UnkSpr_022e153c sprites[38];
    u8 padx[0xeac - 0xe10];
    UnkRes_022e153c *res0;
    UnkRes_022e153c *res1;
    UnkRes_022e153c *res2;
    UnkRes_022e153c *res3;
    UnkRes_022e153c *res4;
} UnkWork_022e153c;

void Background_Init(void *bg, UnkRes_022e153c *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void Background_Draw(void *bg);
s32 ZoneActSelect_GetCompressionType(void *p);
u32 ZoneActSelect_GetUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void ZoneActSelect_InitSprite(UnkSpr_022e153c *s, UnkRes_022e153c *res, u32 idx, u16 v, s32 mode);

#pragma thumb on
void ZoneActSelect_Unk_ov001_022e153c(UnkWork_022e153c *w) {
    UnkRes_022e153c *res;
    s32 i;

    res = w->res0;
    Background_Init(w->bg0, res, 0x38, 0, 1, res->w, res->h);
    Background_Draw(w->bg0);
    res = w->res1;
    Background_Init(w->bg1, res, 0x38, 0, 2, res->w, res->h);
    Background_Draw(w->bg1);
    res = (UnkRes_022e153c *)((u8 *)res + res->unk10);
    switch (ZoneActSelect_GetCompressionType(res)) {
    case 0x10:
        MI_UncompressLZ16(res, w->buf);
        break;
    case 0x20:
        MI_UncompressHuffman(res, w->buf);
        break;
    case 0x30:
        MI_UncompressRL16(res, w->buf);
        break;
    default:
        Mem_Copy(Compress_GetData(res), w->buf, ZoneActSelect_GetUncompressedSize(res));
        break;
    }
    DC_StoreRange(w->buf, ZoneActSelect_GetUncompressedSize(res));
    res = w->res2;
    ZoneActSelect_InitSprite(&w->sprites[0], res, 0, 1, 1);
    ZoneActSelect_InitSprite(&w->sprites[1], res, 1, 1, 1);
    ZoneActSelect_InitSprite(&w->sprites[2], res, 2, 1, 1);
    ZoneActSelect_InitSprite(&w->sprites[3], res, 3, 2, 1);
    ZoneActSelect_InitSprite(&w->sprites[4], res, 4, 2, 1);
    ZoneActSelect_InitSprite(&w->sprites[5], res, 5, 2, 1);
    ZoneActSelect_InitSprite(&w->sprites[6], res, 6, 3, 1);
    ZoneActSelect_InitSprite(&w->sprites[7], res, 7, 13, 0);
    ZoneActSelect_InitSprite(&w->sprites[8], res, 8, 13, 0);
    ZoneActSelect_InitSprite(&w->sprites[9], res, 15, 4, 0);
    ZoneActSelect_InitSprite(&w->sprites[10], res, 18, 4, 0);
    ZoneActSelect_InitSprite(&w->sprites[11], res, 21, 4, 0);
    ZoneActSelect_InitSprite(&w->sprites[12], res, 25, 0, 0);
    ZoneActSelect_InitSprite(&w->sprites[13], res, 26, 10, 0);
    ZoneActSelect_InitSprite(&w->sprites[14], res, 26, 10, 0);
    ZoneActSelect_InitSprite(&w->sprites[15], res, 26, 10, 0);
    ZoneActSelect_InitSprite(&w->sprites[37], w->res4, 0, 0xe, 0);
    res = w->res3;
    for (i = 0x10; i <= 0x24; i++) {
        ZoneActSelect_InitSprite(&w->sprites[i], res, 0, 7, 0);
    }
}
#pragma thumb reset
