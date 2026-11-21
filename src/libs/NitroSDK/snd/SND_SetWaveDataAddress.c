#include <nitro/types.h>

void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
void DC_StoreRange(void *p, u32 size);
typedef struct { u8 pad[0x3c]; u32 arr[1]; } UnkStruct_02009ba8;
void SND_SetWaveDataAddress(UnkStruct_02009ba8 *p, s32 i, u32 v) { SNDi_LockMutex(); p->arr[i] = v; DC_StoreRange(&p->arr[i], 4); SNDi_UnlockMutex(); }
