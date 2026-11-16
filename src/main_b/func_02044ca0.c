#include <nitro/types.h>

typedef struct { u8 unk0[0x34]; void *unk34; u8 unk38[0x10]; } UnkElem;
typedef struct { u8 unk0[0x10]; UnkElem arr[17]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern u32 data_022c447c;
void ObjVram_Free(s32 a, void *p);
void func_02044ca0(UnkTask *t) { s16 i; UnkElem *e = t->unk10->arr; for (i = 0; i < 17; i++, e++) ObjVram_Free(1, e->unk34); data_022c447c = 0; }
