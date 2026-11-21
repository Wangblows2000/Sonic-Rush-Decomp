#include <nitro/types.h>
#include "game/GameSystem/fileload.h"

void FS_InitFile(UnkFile *f);
BOOL FS_OpenFileFast(UnkFile *f, UnkFileId id);
s32 FS_ReadFileAsync(UnkFile *f, void *dst, s32 len);

void FileRequest_StartRead(UnkLoadNode *n) {
    FS_InitFile(&n->file);
    if (!FS_OpenFileFast(&n->file, n->fileId)) {
        n->status = -2;
        return;
    }
    n->flags |= 2;
    n->size = n->file.bottom - n->file.top;
    if (n->mem == NULL) {
        n->mem = UnkDbgAlloc(n->size, 0x20);
        n->flags |= 1;
    } else if (n->mem == (void *)-1) {
        n->mem = UnkDbgAlloc(n->size, -0x20);
        n->flags |= 1;
    }
    if (n->mem == NULL) {
        n->status = -3;
        return;
    }
    if (FS_ReadFileAsync(&n->file, n->mem, n->size) == -1) {
        n->status = -1;
    } else {
        n->status = 3;
    }
}
