#include <nitro/types.h>

typedef struct { u8 pad[0x14]; s32 a; u8 pad2[0xc]; s32 b; } UnkStruct_022ccbac;
typedef struct { u8 pad[0x10]; UnkStruct_022ccbac *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL func_ov001_022cc17c(void *p);
void func_ov001_022cbf30(void *p);
void func_ov001_022cde84(void);

#pragma thumb on
void OptionsMenu_Main(void) {
    UnkStruct_022ccbac *t = gTaskCurrent->p;
    func_ov001_022cc17c(t);
    func_ov001_022cbf30(t);
    t->a++;
    t->b++;
    if (t->b >= 0x10) {
        func_ov001_022cde84();
    }
}
#pragma thumb reset
