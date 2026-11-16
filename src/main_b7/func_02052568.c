#include <nitro/types.h>

typedef struct {
    u8 pad[0x20];
    u32 unk20;
    u8 pad24[4];
    u32 unk28;
} UnkFile_02052568;

typedef struct {
    u8 pad[8];
    u32 unk8;
    u32 unkC;
    u8 pad10[0x10];
} UnkHdr_02052568;

typedef struct {
    u8 pad[0x1c];
    u8 *buf;
} UnkDst_02052568;

s32 FS_ReadFile(void *f, void *buf, u32 size);
BOOL FS_SeekFile(void *f, u32 pos, u32 whence);

void func_02052568(UnkFile_02052568 *f, UnkDst_02052568 *d, u32 n) {
    UnkHdr_02052568 h;
    u32 size;
    FS_ReadFile(f, &h, 0x20);
    size = (h.unkC - h.unk8 - 4) >> 1;
    FS_SeekFile(f, size * n + 4, 1);
    FS_ReadFile(f, d->buf + size * n, size);
    f->unk28 = f->unk20;
}
