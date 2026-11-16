#include <nitro/types.h>
#include "main_a2/fileload.h"

void func_0200be70(UnkFile *f);
BOOL func_0200bb70(UnkFile *f, UnkFileId id);
s32 func_0200b9ac(UnkFile *f, void *dst, s32 len);

void func_02033b98(UnkLoadNode *n) {
    func_0200be70(&n->file);
    if (!func_0200bb70(&n->file, n->fileId)) {
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
    if (func_0200b9ac(&n->file, n->mem, n->size) == -1) {
        n->status = -1;
    } else {
        n->status = 3;
    }
}
