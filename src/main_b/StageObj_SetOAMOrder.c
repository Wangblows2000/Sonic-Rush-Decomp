#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u8 unk94[0x48]; u8 unkDC[1]; } UnkStruct_02069918;
void SpritePair_SetOAMOrder(void *a, void *b, s16 c);
void StageObj_SetOAMOrder(UnkStruct_02069918 *s, s16 c) { SpritePair_SetOAMOrder(s->unk94, s->unkDC, c); }
