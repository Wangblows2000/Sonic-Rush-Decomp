#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; void *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void func_ov001_022cb770(void *p);
void func_ov001_022cd558(void);

#pragma thumb on
void func_ov001_022cde60(void) {
    func_ov001_022cb770(gTaskCurrent->p);
    gTaskCurrent->fn = func_ov001_022cd558;
}
#pragma thumb reset
