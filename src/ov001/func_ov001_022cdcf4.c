#include <nitro/types.h>

typedef struct { s32 v; } UnkStruct_022cdcf4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cdcf4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void func_ov001_022cb834(void *p);
void func_ov001_022c8aa0(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022ccc6c(void);

#pragma thumb on
void func_ov001_022cdcf4(void) {
    UnkStruct_022cdcf4 *t = gTaskCurrent->p;
    t->v = 0;
    func_ov001_022cb834(t);
    func_ov001_022c8aa0(0, 0x10, 0x10, 0x1e);
    gTaskCurrent->fn = func_ov001_022ccc6c;
}
#pragma thumb reset
