#include <nitro/types.h>

typedef struct {
    u8 unk0[0x20];
    u32 top;
    u32 bottom;
    u32 pos;
    u8 unk2C[0x18];
} UnkFile_02031f30;

typedef struct {
    void *arc;
    u32 id;
} UnkFileId_02031f30;

typedef struct {
    u8 unk0[0x14];
    u32 dataOfs;
    u8 unk18[4];
} UnkHdr_02031f30;

typedef struct {
    u32 ofs;
    u16 size;
    u16 pad;
} UnkEntry_02031f30;

typedef struct {
    u16 unk0;
    u16 len;
    UnkEntry_02031f30 entries[1];
} UnkCmd_02031f30;

typedef struct {
    u32 unk0;
    u32 tableOfs;
    u32 unk8;
    u32 infoOfs;
    u32 unk10;
    u32 dataOfs;
} UnkRes_02031f30;

typedef struct {
    u16 type;
    u16 pad;
    u32 unk4;
} UnkTblEntry_02031f30;

typedef struct {
    u8 unk0[8];
    UnkTblEntry_02031f30 entries[1];
} UnkTbl_02031f30;

typedef struct {
    u8 unk0[0x10];
    u16 flags;
} UnkInfo_02031f30;

typedef struct {
    u8 unk0[0x94];
    u16 unk94;
    u16 unk96;
} UnkScr_02031f30;

typedef struct {
    u32 idx;
    u8 unk4[4];
    u16 unk8;
    u8 unkA[6];
    UnkRes_02031f30 *res;
    UnkFileId_02031f30 fileId;
    u32 pos;
    u8 unk20[4];
    u32 unk24;
    u8 unk28[4];
    u32 flags;
    u32 unk30;
    u32 dst;
    u8 *buf;
} UnkCtx_02031f30;

typedef void (*UnkFn_02031f30)(u32 src, u32 mode, u32 dst);

extern UnkScr_02031f30 *gDisplayTable[];
extern u8 data_02072e64[];

void VRAMTransfer_LoadNow(u32 a, u32 b, u32 c);
void VRAMTransfer_QueueLoad(u32 a, u32 b, u32 c);
void FS_InitFile(UnkFile_02031f30 *f);
BOOL FS_OpenFileFast(UnkFile_02031f30 *f, UnkFileId_02031f30 id);
s32 FS_ReadFile(UnkFile_02031f30 *f, void *dst, s32 len);
BOOL FS_SeekFile(UnkFile_02031f30 *f, s32 ofs, s32 whence);
BOOL FS_CloseFile(UnkFile_02031f30 *f);

static inline UnkTbl_02031f30 *GetTbl_02031f30(UnkRes_02031f30 *r)
{
    return (UnkTbl_02031f30 *)((u32)r + r->tableOfs);
}

BOOL func_02031f30(UnkCmd_02031f30 *cmd, UnkCtx_02031f30 *c)
{
    u32 flags = c->flags;
    if (!(flags & 8)) {
        UnkFn_02031f30 fn;
        UnkEntry_02031f30 *e;
        u32 shift;
        u16 k;
        u32 dst;
        u32 bpp;
        if (flags & 0x20)
            fn = VRAMTransfer_LoadNow;
        else
            fn = VRAMTransfer_QueueLoad;
        e = cmd->entries;
        dst = c->dst;
        k = 4;
        if (c->unk30 == 0) {
            UnkInfo_02031f30 *info = (UnkInfo_02031f30 *)(c->unk24 + ((u32)c->res + c->res->infoOfs));
            if ((info->flags & 0xc00) == 0xc00)
                shift = gDisplayTable[c->idx]->unk96;
            else
                shift = gDisplayTable[c->idx]->unk94;
        } else {
            shift = 0;
        }
        bpp = data_02072e64[GetTbl_02031f30(c->res)->entries[c->unk8].type];
        if (flags & 0x8000) {
            UnkFile_02031f30 file;
            UnkHdr_02031f30 hdr;
            u32 ofs = 0;
            u32 base;
            FS_InitFile(&file);
            if (!FS_OpenFileFast(&file, c->fileId)) {
                c->pos += cmd->len;
                return TRUE;
            }
            FS_ReadFile(&file, &hdr, sizeof(hdr));
            FS_SeekFile(&file, hdr.dataOfs, 0);
            base = file.pos - file.top;
            for (; k < cmd->len;) {
                FS_SeekFile(&file, base + e->ofs, 0);
                FS_ReadFile(&file, c->buf + ofs, 4);
                FS_ReadFile(&file, c->buf + ofs + 4, *(u32 *)(c->buf + ofs) >> 8);
                fn((u32)(c->buf + ofs), c->unk30, dst);
                dst += ((e->size + (1u << shift) - 1) >> shift) << (shift + bpp);
                e++;
                k += 8;
                ofs += (*(u32 *)(c->buf + ofs) >> 8) + 4;
            }
            FS_CloseFile(&file);
        } else {
            for (; k < cmd->len; k += 8, e++) {
                fn(e->ofs + ((u32)c->res + c->res->dataOfs), c->unk30, dst);
                dst += ((e->size + (1u << shift) - 1) >> shift) << (shift + bpp);
            }
        }
    }
    c->pos += cmd->len;
    return TRUE;
}
