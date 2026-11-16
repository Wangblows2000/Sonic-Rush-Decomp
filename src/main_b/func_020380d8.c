#include <nitro/types.h>

typedef struct { u8 unk0[8]; s32 unk8; u8 unkC[8]; s32 unk14; } UnkStruct_020380d8;
UnkStruct_020380d8 *NNS_G3dGetTex(void *p);
s32 func_020380d8(UnkStruct_020380d8 *s) { UnkStruct_020380d8 *r = NNS_G3dGetTex(s); if (r != NULL) return (u8 *)r + r->unk14 - (u8 *)s; return s->unk8; }
