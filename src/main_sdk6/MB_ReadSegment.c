#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    char *arc;  /* 0x08 */
    u8 padC[0x14];
    u32 top;    /* 0x20 */
    u8 pad24[4];
    u32 pos;    /* 0x28 */
    u8 pad2c[0x44 - 0x2c];
} UnkFSFile_020174a4;

typedef struct {
    u8 pad0[0x20];
    u32 arm9RomOffset; /* 0x20 */
    u8 pad24[4];
    u32 arm9RamAddr;   /* 0x28 */
    u32 arm9Size;      /* 0x2c */
    u32 arm7RomOffset; /* 0x30 */
    u8 pad34[8];
    u32 arm7Size;      /* 0x3c */
    u8 pad40[0x20];
    u32 unk60;         /* 0x60 */
    u8 pad64[0x1c];
    u32 romSize;       /* 0x80 */
} UnkRomHeader_020174a4;

typedef struct {
    u8 pad0[0x10];
    char name[4];  /* 0x10 */
    u32 nameLen;   /* 0x14 */
    u8 pad18[0x30];
    u8 *unk48;     /* 0x48 */
} UnkSegInfo_020174a4;

typedef struct {
    u32 unk0;
    u32 arm9Offset;
    u32 arm7Offset;
    u32 unkC;
} UnkSegHdr_020174a4;

typedef struct {
    u32 base;
    u8 *src;
    u8 *dst;
    u32 size;
} UnkBuf_020174a4;

typedef struct {
    u32 start;
    u32 size;
} UnkRange_020174a4;

extern UnkRange_020174a4 *data_020779b8;
extern char data_020779bc[];
void AutoloadCallback(void);

s32 FS_ReadFile(UnkFSFile_020174a4 *f, void *dst, u32 size);
BOOL FS_SeekFile(UnkFSFile_020174a4 *f, s32 offset, u32 origin);
void FS_InitFile(UnkFSFile_020174a4 *f);
void *FS_FindArchive(const char *name, u32 len);
BOOL FS_OpenFileDirect(UnkFSFile_020174a4 *f, void *arc, u32 top, u32 bottom, s32 id);
void FS_CloseFile(UnkFSFile_020174a4 *f);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MBi_InitCache(void *p);
void MBi_AttachCacheBuffer(UnkSegInfo_020174a4 *s, u32 base, u32 size, void *ptr, u32 state);
void MBi_ReadSegmentHeader(UnkBuf_020174a4 *b, u32 start, u32 end, BOOL clear);
void DC_FlushRange(void *p, u32 size);

#pragma opt_dead_assignments off
static inline void S_020174a4(u32 v, u32 ofs, u8 *base) { *(u32 *)(base + ofs) = v; }
BOOL MB_ReadSegment(UnkFSFile_020174a4 *file, UnkRomHeader_020174a4 *buf, u32 len)
{
    UnkFSFile_020174a4 tmp;
    BOOL ret = FALSE;
    u32 rest;
    u8 *p;
    BOOL opened;
    UnkSegInfo_020174a4 *seg;
    UnkSegHdr_020174a4 *hdr;
    s32 origin;
    u32 romSize;

    if (len >= 0x164) {
        rest = len;
        p = (u8 *)buf;
        opened = FALSE;
        seg = NULL;
        hdr = NULL;
        p += 0x160;
        rest -= 0x160;
        if (file != NULL) {
            origin = file->pos - file->top;
            if (FS_ReadFile(file, buf, 0x160) < 0x160) {
                rest = 0;
            }
            romSize = buf->romSize;
            if (romSize == 0) {
                romSize = 0x1000000;
            }
        } else {
            UnkRomHeader_020174a4 *rom = (UnkRomHeader_020174a4 *)0x027ffe00;
            romSize = rom->romSize;
            if (romSize == 0) {
                romSize = 0x1000000;
            }
            opened = TRUE;
            FS_InitFile(&tmp);
            FS_OpenFileDirect(&tmp, FS_FindArchive(data_020779bc, 3), 0, romSize + 0x88, -1);
            file = &tmp;
            origin = file->pos - file->top;
            MI_CpuCopy8(rom, buf, 0x160);
            buf->unk60 |= 0x406000;
        }
        if (rest >= 0x88) {
            FS_SeekFile(file, origin + romSize, 0);
            FS_ReadFile(file, p, 0x88);
            p += 0x88;
            rest -= 0x88;
        } else {
            rest = 0;
        }
        if (rest >= 0x70) {
            char *name;
            s32 i;
            seg = (UnkSegInfo_020174a4 *)p;
            MBi_InitCache(p);
            p += 0x70;
            rest -= 0x70;
            MBi_AttachCacheBuffer(seg, 0, 0x160, buf, 3);
            name = file->arc;
            for (i = 0; i < 3 && name[i] != 0; i++) {
            }
            MI_CpuCopy8(name, seg->name, i);
            seg->nameLen = i;
        } else {
            rest = 0;
        }
        if (rest >= 0x10) {
            ((UnkSegHdr_020174a4 *)p)->unk0 = 0;
            ((UnkSegHdr_020174a4 *)p)->arm9Offset = buf->arm9RomOffset + (origin + file->top);
            ((UnkSegHdr_020174a4 *)p)->arm7Offset = buf->arm7RomOffset + (origin + file->top);
            hdr = (UnkSegHdr_020174a4 *)p;
            p += 0x10;
            rest -= 0x10;
        } else {
            rest = 0;
        }
        if (rest >= buf->arm9Size + buf->arm7Size) {
            u32 top = file->top;
            FS_SeekFile(file, hdr->arm9Offset - top, 0);
            FS_ReadFile(file, p, buf->arm9Size);
            MBi_AttachCacheBuffer(seg, hdr->arm9Offset, buf->arm9Size, p, 3);
            p += buf->arm9Size;
            FS_SeekFile(file, hdr->arm7Offset - top, 0);
            FS_ReadFile(file, p, buf->arm7Size);
            MBi_AttachCacheBuffer(seg, hdr->arm7Offset, buf->arm7Size, p, 3);
            ret = TRUE;
        } else if (rest >= 0xcc00) {
            u32 top = file->top;
            u32 off = hdr->arm9Offset;
            FS_SeekFile(file, off - top, 0);
            FS_ReadFile(file, p, 0x4400);
            MBi_AttachCacheBuffer(seg, off, 0x4400, p, 3);
            FS_SeekFile(file, off + 0x4400 - top, 0);
            FS_ReadFile(file, p + 0x4400, 0x4400);
            MBi_AttachCacheBuffer(seg, off + 0x4400, 0x4400, p + 0x4400, 2);
            FS_SeekFile(file, off + 0x8800 - top, 0);
            FS_ReadFile(file, p + 0x8800, 0x4400);
            MBi_AttachCacheBuffer(seg, off + 0x8800, 0x4400, p + 0x8800, 2);
            ret = TRUE;
        }
        FS_SeekFile(file, origin, 0);
        if (opened) {
            FS_CloseFile(&tmp);
            if (ret) {
                UnkBuf_020174a4 b;
                UnkRange_020174a4 *r = data_020779b8;
                b.base = buf->arm9RomOffset;
                b.src = (u8 *)(buf->arm9RamAddr - buf->arm9RomOffset);
                b.dst = seg->unk48 - buf->arm9RomOffset;
                b.size = len;
                MBi_ReadSegmentHeader(&b, 0x4000, 0x8000, TRUE);
                for (; ((u32 *)r)[1] != 0; r++) {
                    MBi_ReadSegmentHeader(&b, r->start, r->start + r->size, FALSE);
                }
                S_020174a4(0xe12fff1e, -buf->arm9RamAddr + (u32)AutoloadCallback, seg->unk48);
            }
        }
        if (ret) {
            DC_FlushRange(buf, len);
        }
    }
    return ret;
}
