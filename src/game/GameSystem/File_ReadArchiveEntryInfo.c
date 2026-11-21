#include <nitro/types.h>

void FS_SeekFile(void *a, s32 b, s32 c);
void FS_ReadFile(void *a, void *b, s32 c);
void File_ReadArchiveEntryInfo(void *a, s32 n, void *b) { FS_SeekFile(a, n * 8 + 8, 0); FS_ReadFile(a, b, 8); }
