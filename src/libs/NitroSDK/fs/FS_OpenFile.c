#include <nitro/types.h>

typedef struct {
    u32 a;
    u32 b;
} UnkStruct_0200bb20;

BOOL FS_ConvertPathToFileID(UnkStruct_0200bb20 *out);
BOOL FS_OpenFileFast(void *p, UnkStruct_0200bb20 v);

BOOL FS_OpenFile(void *p)
{
    UnkStruct_0200bb20 v;
    if (FS_ConvertPathToFileID(&v) && FS_OpenFileFast(p, v)) {
        return TRUE;
    }
    return FALSE;
}
