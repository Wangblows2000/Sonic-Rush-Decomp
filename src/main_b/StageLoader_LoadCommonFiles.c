#include <nitro/types.h>

extern u8 gStageCommonFileList;
void StageLoader_ProcessFileList(void *p, s32 a);
void StageLoader_LoadCommonFiles(void) { StageLoader_ProcessFileList(&gStageCommonFileList, 1); }
