#include <nitro/types.h>
#include "game/GameSystem/fileload.h"

extern UnkLoadNode *gFileRequestListTail;

UnkLoadNode *FileRequest_AllocNode(void);
BOOL FS_ConvertPathToFileID(UnkFileId *id, const char *path);

UnkLoadNode *FileRequest_Create(const char *path, void *mem)
{
    UnkLoadNode *n = FileRequest_AllocNode();
    n->prev = gFileRequestListTail->prev;
    n->next = gFileRequestListTail;
    n->prev->next = n;
    gFileRequestListTail->prev = n;
    n->status = 1;
    n->flags = 0;
    n->mem = mem;
    n->size = 0;
    if (!FS_ConvertPathToFileID(&n->fileId, path))
        n = NULL;
    return n;
}
