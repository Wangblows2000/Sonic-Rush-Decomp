#include <nitro/types.h>

typedef struct { u8 pad[4]; u8 *p; u8 pad2[4]; u8 a; u8 b; u8 c; } UnkStruct_02080e74;
extern UnkStruct_02080e74 data_02080e74;
void MBi_InitSendVolatBeacon(void) { *(u16 *)(data_02080e74.p + 0x4b0) = 1; data_02080e74.c = data_02080e74.p[0x4b4]; data_02080e74.a = 5; }
