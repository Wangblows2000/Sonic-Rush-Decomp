#include <nitro/types.h>

typedef struct { u8 pad[0x14]; s32 a; } UnkStruct_022cdbc4;
typedef struct { u8 pad[0x10]; UnkStruct_022cdbc4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
BOOL func_ov001_022cc17c(void *p);
void func_ov001_022cbf30(void *p);
void func_ov001_022cdfb4(void);

#pragma thumb on
void func_ov001_022cdbc4(void) {
    UnkStruct_022cdbc4 *t = data_022b4574->p;
    BOOL r = func_ov001_022cc17c(t);
    func_ov001_022cbf30(t);
    t->a++;
    if (r) {
        func_ov001_022cdfb4();
    }
}
#pragma thumb reset
