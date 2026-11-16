#include <nitro/types.h>

#pragma thumb on

extern u8 data_0207c450;
void File_LoadArchiveEntry(void *p, s32 a, s32 b);
void func_02056f90(void) { File_LoadArchiveEntry(&data_0207c450, 0, -1); }

#pragma thumb reset
