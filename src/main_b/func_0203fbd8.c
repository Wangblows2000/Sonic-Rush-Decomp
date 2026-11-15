#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fbd8;
extern void (*data_022c197c)(void *);
extern u8 data_020789dc;
void func_020405bc(s32 v);
void func_0203dda4(void);
BOOL func_0203fb9c(void);
void func_0203fbd8(UnkStruct_0203fbd8 *s) { if (s->unk2 != 0) { func_020405bc(s->unk2); func_0203dda4(); return; } if (func_0203fb9c()) return; if (data_022c197c != NULL) data_022c197c(&data_020789dc); func_0203dda4(); }
