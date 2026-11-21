#include <nitro/types.h>

BOOL FS_UnloadArchive(void *p);
void FS_ReleaseArchiveName(void *p);

BOOL NNS_FndUnmountArchive(void *p) {
    if (!FS_UnloadArchive(p)) return FALSE;
    FS_ReleaseArchiveName(p);
    return TRUE;
}
