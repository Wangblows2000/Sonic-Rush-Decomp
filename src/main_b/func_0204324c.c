#include <nitro/types.h>

typedef struct { u8 unk0[0x1e]; } UnkElem;
typedef struct { u8 unk0[0x192c]; UnkElem arr[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
UnkElem *func_0204324c(s32 i) { return &data_022c4364->unk10->arr[i - 1]; }
