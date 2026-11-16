#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;
    u8 pad24[4];
    u32 pos;
} UnkFSFile_02017910;

u32 FS_ReadFile(UnkFSFile_02017910 *f, void *dst, u32 size);
BOOL FS_SeekFile(UnkFSFile_02017910 *f, s32 offset, u32 origin);

u32 MB_GetSegmentLength(UnkFSFile_02017910 *file)
{
    u8 hdr[0x60];
    u8 *h = NULL;
    u32 r = 0;
    if (file != NULL) {
        s32 pos = file->pos - file->top;
        if (FS_ReadFile(file, hdr, 0x60) >= 0x60) {
            h = hdr;
        }
        FS_SeekFile(file, pos, 0);
    } else {
        h = (u8 *)0x027ffe00;
    }
    if (h != NULL) {
        r = *(u32 *)(h + 0x2c) + 0x268 + *(u32 *)(h + 0x3c);
        if (r < 0x10000) {
            r = 0x10000;
        }
    }
    return r;
}
