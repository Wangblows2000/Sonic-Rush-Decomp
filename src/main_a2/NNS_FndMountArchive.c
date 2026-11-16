#include <nitro/types.h>

typedef struct { u32 signature; u32 size; } UnkBlock_02021310;
typedef struct { u8 unk0[0xC]; u16 headerSize; u16 numBlocks; } UnkNarc_02021310;
typedef struct { u8 unk0[0x50]; void *narc; void *fat; void *img; } UnkArc_02021310;

BOOL IsValidArchiveBinary(void *narc);
void FS_InitArchive(UnkArc_02021310 *arc);
u32 strlen(const char *s);
BOOL FS_RegisterArchiveName(UnkArc_02021310 *arc, const char *name, u32 len);
BOOL FS_LoadArchive(UnkArc_02021310 *arc, u32 base, u32 fatOff, u32 fatSize, u32 fntOff, u32 fntSize, void *read, void *write);
void FS_ReleaseArchiveName(UnkArc_02021310 *arc);

BOOL NNS_FndMountArchive(UnkArc_02021310 *arc, const char *name, UnkNarc_02021310 *narc) {
    UnkBlock_02021310 *fat = NULL;
    UnkBlock_02021310 *fnt = NULL;
    UnkBlock_02021310 *img = NULL;
    s32 i;
    UnkBlock_02021310 *blk;
    u32 base;
    if (!IsValidArchiveBinary(narc)) return FALSE;
    blk = (UnkBlock_02021310 *)((u8 *)narc + narc->headerSize);
    for (i = 0; i < narc->numBlocks; i++) {
        switch (blk->signature) {
        case 0x46415442:
            fat = blk;
            break;
        case 0x464E5442:
            fnt = blk;
            break;
        case 0x46494D47:
            img = blk;
            break;
        }
        blk = (UnkBlock_02021310 *)((u8 *)blk + blk->size);
    }
    FS_InitArchive(arc);
    arc->narc = narc;
    arc->fat = fat;
    base = (u32)img + 8;
    arc->img = (void *)base;
    if (!FS_RegisterArchiveName(arc, name, strlen(name))) return FALSE;
    if (FS_LoadArchive(arc, base, (u32)fat + 0xC - base, fat->size - 0xC, (u32)fnt + 8 - base, fnt->size - 8, NULL, NULL)) return TRUE;
    FS_ReleaseArchiveName(arc);
    return FALSE;
}
