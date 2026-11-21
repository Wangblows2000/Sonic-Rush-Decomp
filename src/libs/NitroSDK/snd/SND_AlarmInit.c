#include <nitro/types.h>

typedef struct { void *cb; void *arg; u8 id; u8 pad[3]; } UnkStruct_0200981c;
extern UnkStruct_0200981c data_0207eec0[];
void SND_AlarmInit(void) { s32 i; for (i = 0; i < 8; i++) { data_0207eec0[i].cb = 0; data_0207eec0[i].arg = 0; data_0207eec0[i].id = 0; } }
