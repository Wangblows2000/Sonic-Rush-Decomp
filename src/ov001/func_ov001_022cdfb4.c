#include <nitro/types.h>

typedef struct { u8 pad[0x10]; s32 v; } UnkStruct_022cdfb4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cdfb4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void func_ov001_022cbb7c(void *p);
void func_ov001_022cdabc(void);

#pragma thumb on
void func_ov001_022cdfb4(void) {
    UnkStruct_022cdfb4 *t = gTaskCurrent->p;
    t->v = 0;
    func_ov001_022cbb7c(t);
    gTaskCurrent->fn = func_ov001_022cdabc;
}
#pragma thumb reset
