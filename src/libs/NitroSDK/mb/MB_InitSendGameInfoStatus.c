#include <nitro/types.h>

typedef struct { u32 a; u32 b; u32 pad; u8 c; } UnkStruct_02080e74;
extern UnkStruct_02080e74 data_02080e74;
extern u32 data_02080e70;
void MBi_ClearSendStatus(void);
void MB_InitSendGameInfoStatus(void) { data_02080e74.a = 0; data_02080e74.b = 0; data_02080e74.c = 1; data_02080e70 = 0; MBi_ClearSendStatus(); }
