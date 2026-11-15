#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fc88;
extern void (*data_022c197c)(void *);
extern u8 data_020789f8;
void func_020405bc(s32 v);
void func_0203dda4(void);
BOOL func_0203fc44(void);
void func_0203fc88(UnkStruct_0203fc88 *s) { if (s->unk2 != 0) { func_020405bc(s->unk2); func_0203dda4(); return; } if (func_0203fc44()) return; if (data_022c197c != NULL) data_022c197c(&data_020789f8); func_0203dda4(); }
