#include <nitro/types.h>
#include "main_a2/fileload.h"

extern UnkLoadNode *data_0208f7e4;

UnkLoadNode *func_02033db0(void);
BOOL func_0200bc44(UnkFileId *id, const char *path);

UnkLoadNode *func_02033f44(const char *path, void *mem)
{
    UnkLoadNode *n = func_02033db0();
    n->prev = data_0208f7e4->prev;
    n->next = data_0208f7e4;
    n->prev->next = n;
    data_0208f7e4->prev = n;
    n->status = 1;
    n->flags = 0;
    n->mem = mem;
    n->size = 0;
    if (!func_0200bc44(&n->fileId, path))
        n = NULL;
    return n;
}
