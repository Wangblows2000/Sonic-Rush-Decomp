#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; s32 unk4; } UnkTask;
extern UnkTask *data_022c465c;
void func_02051d34(s32 i);
void func_02051b7c(s32 i);
void func_02052000(s32 i);
void func_02052168(s32 i) { if (data_022c465c != NULL && i != data_022c465c->unk4) { if (i < 6) { func_02051d34(i); func_02051b7c(i); } else func_02052000(i); data_022c465c->unk4 = i; } }

#pragma thumb reset
