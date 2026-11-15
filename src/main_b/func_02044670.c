#include <nitro/types.h>

typedef struct { u8 unk0[6]; s8 unk6; s8 unk7; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4478;
s8 func_02044670(void) { if (data_022c4478 == NULL) return 0; return data_022c4478->unk10->unk7; }
