#include <nitro/types.h>

typedef struct { u8 pad[0x14]; s32 a; } UnkStruct_022ccddc;
typedef struct { u8 pad[0x10]; UnkStruct_022ccddc *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL func_ov001_022cc17c(void *p);
void func_ov001_022cbf30(void *p);
void func_ov001_022cdfb4(void);

#pragma thumb on
void func_ov001_022ccddc(void) {
    UnkStruct_022ccddc *t = gTaskCurrent->p;
    BOOL r = func_ov001_022cc17c(t);
    func_ov001_022cbf30(t);
    t->a++;
    if (r) {
        func_ov001_022cdfb4();
    }
}
#pragma thumb reset
