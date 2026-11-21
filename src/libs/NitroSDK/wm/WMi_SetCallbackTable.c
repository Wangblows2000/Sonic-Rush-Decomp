#include <nitro/types.h>

typedef struct { u8 pad[0x18]; u32 arr[1]; } UnkStruct_02010cd4;
extern UnkStruct_02010cd4 *data_0207faec;
void WMi_SetCallbackTable(u32 i, u32 v) { data_0207faec->arr[i] = v; }
