#include <nitro/types.h>

typedef struct { u8 unk0[0x4c]; u8 unk4C; u8 unk4D; } UnkStruct_022c4490;
extern UnkStruct_022c4490 gStageLoader;
typedef struct { u32 unk0; u32 unk4; } UnkElem;
typedef struct { UnkElem arr[3]; } UnkRow;
extern UnkRow gStageZoneFileListTable[];
s32 StageLoader_ProcessFileList(void *p, s32 a);
s32 StageLoader_LoadZoneFiles(void) { return StageLoader_ProcessFileList(&gStageZoneFileListTable[gStageLoader.unk4C].arr[gStageLoader.unk4D], 0); }
