#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x28]; u32 unk28; } UnkTask;
extern UnkTask *data_022c465c;
void func_02051d34(s32 i);
void func_02051b7c(s32 i);
void func_02052000(s32 i);
void func_02052128(s32 i) { if (data_022c465c != NULL) { if (i < 6) { func_02051d34(i); data_022c465c->unk28 = 1; func_02051b7c(i); data_022c465c->unk28 = 0; return; } func_02052000(i); } }

#pragma thumb reset
