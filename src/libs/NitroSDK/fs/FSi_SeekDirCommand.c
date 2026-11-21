#include <nitro/types.h>

typedef struct {
    void *arc;
    u16 ownId;
    u16 index;
    u32 pos;
} UnkFSDirPos_0200aac4;

typedef struct {
    u32 start;
    u16 index;
    u16 parent;
} UnkFSFNT_0200aac4;

typedef struct {
    u8 pad0[0x28];
    u32 fnt;   /* 0x28 */
} UnkFSArchive_0200aac4;

typedef struct {
    void *arc;
    u32 pos;
} UnkFSReadParam_0200aac4;

typedef struct {
    u8 pad0[8];
    UnkFSArchive_0200aac4 *arc;    /* 0x08 */
    u8 padC[0x10];
    UnkFSDirPos_0200aac4 dirPos;   /* 0x1c */
    u32 parent;                    /* 0x28 */
    UnkFSDirPos_0200aac4 arg;      /* 0x2c */
} UnkFSFile_0200aac4;

void FSi_ReadTable(UnkFSReadParam_0200aac4 *p, void *dst, u32 len);

s32 FSi_SeekDirCommand(UnkFSFile_0200aac4 *p_dir)
{
    UnkFSArchive_0200aac4 *const p_arc = p_dir->arc;
    const UnkFSDirPos_0200aac4 *const arg = &p_dir->arg;
    UnkFSFNT_0200aac4 fnt;
    UnkFSReadParam_0200aac4 param;

    param.arc = p_arc;
    param.pos = p_arc->fnt + arg->ownId * sizeof(fnt);
    FSi_ReadTable(&param, &fnt, sizeof(fnt));
    p_dir->dirPos = *arg;
    if ((arg->index == 0) && (arg->pos == 0)) {
        p_dir->dirPos.index = fnt.index;
        p_dir->dirPos.pos = p_arc->fnt + fnt.start;
    }
    p_dir->parent = (u32)(fnt.parent & 0xfff);
    return 0;
}
