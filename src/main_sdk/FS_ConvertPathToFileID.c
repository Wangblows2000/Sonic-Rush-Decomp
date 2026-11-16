#include <nitro/types.h>

typedef struct { u8 data[0x44]; } UnkStruct_0200bc44;
void FS_InitFile(UnkStruct_0200bc44 *p);
s32 FSi_FindPath(UnkStruct_0200bc44 *p, u32 a, u32 b, u32 c);
BOOL FS_ConvertPathToFileID(u32 b, u32 a) { UnkStruct_0200bc44 s; FS_InitFile(&s); return FSi_FindPath(&s, a, b, 0) ? 1 : 0; }
