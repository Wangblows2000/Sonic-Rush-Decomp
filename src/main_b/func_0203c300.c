#include <nitro/types.h>

extern void *data_022b8bc0;
extern void *data_022b8be0;
typedef struct { u8 unk0[0x858]; u32 unk858; } UnkStruct_022c0be0;
extern UnkStruct_022c0be0 data_022c0be0;
u32 func_0203c300(void) { if (data_022b8bc0 == NULL || data_022b8be0 == NULL) return 0; return data_022c0be0.unk858; }
