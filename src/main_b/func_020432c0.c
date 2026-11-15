#include <nitro/types.h>

typedef struct { u8 unk0[0xc8]; } UnkElem;
typedef struct { u8 unk0[0x28]; UnkElem arr[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
UnkElem *func_020432c0(s32 i) { return &data_022c4364->unk10->arr[i]; }
