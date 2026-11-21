#include <nitro/types.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02364f04[];

void NNS_FndMountArchive(void *ctx, void *a, void *b);
void *NNS_FndGetArchiveFileByIndex(void *ctx, u32 id);
void NNS_FndUnmountArchive(void *ctx);
void *NNS_G3dGetTex(s32 a);
s32 Asset3D_GetPaletteKeyByName(void *h, u32 a);
void PaletteTransfer_QueueCopy(void *res, u32 a, s32 b, s32 c);

void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n) {
    u8 ctx[0x5c];
    void *h;
    void *r;
    NNS_FndMountArchive(ctx, data_ov000_02364f04, gZoneActArchive);
    h = NNS_G3dGetTex(a);
    while (n--) {
        r = NNS_FndGetArchiveFileByIndex(ctx, *p2);
        PaletteTransfer_QueueCopy(r, *p3, 5, Asset3D_GetPaletteKeyByName(h, *p1));
        p1++;
        p2++;
        p3++;
    }
    NNS_FndUnmountArchive(ctx);
}
