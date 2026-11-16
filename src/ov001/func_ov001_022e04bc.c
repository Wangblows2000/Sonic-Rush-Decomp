#include <nitro/types.h>

typedef struct UnkStruct_022e04bc { void (*fn)(struct UnkStruct_022e04bc *); u32 flags; s32 count; } UnkStruct_022e04bc;
typedef struct { u8 pad[0x10]; UnkStruct_022e04bc *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
extern u16 gPad[];
void Task_Destroy(void *p);
void func_ov001_022de14c(void *p, void *q);
void func_ov001_022dde00(void *p, void *q);
void func_ov001_022df878(void *p);

void func_ov001_022e04bc(void) {
    UnkStruct_022e04bc *t = gTaskCurrent->p;
    u32 f;
    t->count++;
    f = t->flags;
    if ((f & 0x100) && !(f & 0x600) && (gPad[2] & 8)) {
        t->flags = f | 0x200;
    }
    if (t->flags & 0x40) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (t->fn != NULL) {
        t->fn(t);
    }
    func_ov001_022de14c(t, (u8 *)t + 0xea0);
    func_ov001_022dde00(t, (u8 *)t + 0xea8);
    func_ov001_022df878(t);
    if (t->flags & 0x80000000) {
        Task_Destroy(gTaskCurrent);
    }
}
