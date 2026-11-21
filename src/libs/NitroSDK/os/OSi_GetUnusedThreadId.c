#include <nitro/types.h>

typedef struct { u8 pad[0x14]; void *slots[16]; } UnkStruct_02006094;
extern UnkStruct_02006094 data_0207d090;
s32 OSi_GetUnusedThreadId(void) { s32 i; for (i = 0; i < 16; i++) { if (data_0207d090.slots[i] == 0) return i; } return -1; }
