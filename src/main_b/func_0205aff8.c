#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); } UnkStruct_0205aff8;
void func_0205aae4(void);
void func_0205aff8(UnkStruct_0205aff8 *s) { s->fn = func_0205aae4; }
