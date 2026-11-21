#include <nitro/types.h>

void FSi_ReadFileCore(u32 a, u32 b, u32 c, u32 d);
void FS_ReadFileAsync(u32 a, u32 b, u32 c) { FSi_ReadFileCore(a, b, c, 1); }
