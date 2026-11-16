#include <nitro/types.h>

typedef struct { u8 pad[0x28]; s32 v; } UnkStruct_022cddc4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cddc4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void func_ov001_022cb770(void *p);
void func_ov001_022cce10(void);

#pragma thumb on
void func_ov001_022cddc4(void) {
    UnkStruct_022cddc4 *t = gTaskCurrent->p;
    func_ov001_022cb770(t);
    t->v = -1;
    gTaskCurrent->fn = func_ov001_022cce10;
}
#pragma thumb reset
