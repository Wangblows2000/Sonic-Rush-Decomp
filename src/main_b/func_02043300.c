#include <nitro/types.h>

typedef struct { u8 unk0[0x1928]; u16 unk1928; } UnkStruct_02043300;
typedef struct { u8 unk0[0x10]; UnkStruct_02043300 *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u16 func_02043300(void) { return data_022c4364->unk10->unk1928; }
