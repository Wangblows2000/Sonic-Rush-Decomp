#include <nitro/types.h>

typedef struct UnkStruct_0200fee4 { void (*fn)(struct UnkStruct_0200fee4 *); } UnkStruct_0200fee4;
extern UnkStruct_0200fee4 data_0207f8c0;
BOOL func_02010358(UnkStruct_0200fee4 *p);
void func_020102d8(void);
void func_0200fee4(void) { UnkStruct_0200fee4 *p = &data_0207f8c0; if (func_02010358(p)) p->fn(p); func_020102d8(); }
