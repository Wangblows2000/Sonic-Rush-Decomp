#include <nitro/types.h>

typedef struct { u8 pad[0x28]; s32 v; } UnkStruct_022cddc4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cddc4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_ov001_022cb770(void *p);
void func_ov001_022cce10(void);

#pragma thumb on
void func_ov001_022cddc4(void) {
    UnkStruct_022cddc4 *t = data_022b4574->p;
    func_ov001_022cb770(t);
    t->v = -1;
    data_022b4574->fn = func_ov001_022cce10;
}
#pragma thumb reset
