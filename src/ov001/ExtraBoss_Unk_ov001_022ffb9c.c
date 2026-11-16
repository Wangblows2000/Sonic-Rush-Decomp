#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern u8 data_ov001_0231d9e4[];
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);
void *NNS_G3dGetTex(s32 a);
s32 Asset3D_GetPaletteKeyByName(void *a, s32 b);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, s32 c);

void ExtraBoss_Unk_ov001_022ffb9c(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n) {
    UnkStruct_023029d4 arc;
    void *h;
    NNS_FndMountArchive(&arc, data_ov001_0231d9e4, b);
    h = NNS_G3dGetTex(a);
    while (n-- != 0) {
        void *res = NNS_FndGetArchiveFileByIndex(&arc, *d);
        PaletteTransfer_QueueCopy(res, *e, 5, Asset3D_GetPaletteKeyByName(h, *c));
        c++;
        d++;
        e++;
    }
    NNS_FndUnmountArchive(&arc);
}
