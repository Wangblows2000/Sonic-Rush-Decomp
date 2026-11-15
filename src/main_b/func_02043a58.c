#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *data_022c4410;
BOOL func_02043a58(void) { if (data_022c4410 == NULL) return TRUE; return (*data_022c4410->unk10 & 0x20) != 0; }
