#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 timer;
    u8 pad18[0xd08 - 0x18];
    s32 count;
} UnkStruct_022d1840;

void func_ov001_022d0070(UnkStruct_022d1840 *p);
void func_ov001_022d2234(UnkStruct_022d1840 *p, void (*fn)(UnkStruct_022d1840 *));
void func_ov001_022d1710(UnkStruct_022d1840 *p);
void func_ov001_022d17bc(UnkStruct_022d1840 *p);

#pragma thumb on
void func_ov001_022d1840(UnkStruct_022d1840 *p) {
    func_ov001_022d0070(p);
    if (p->timer > 0x12c) {
        if (p->count + 1 >= 4) {
            func_ov001_022d2234(p, func_ov001_022d1710);
            return;
        }
        func_ov001_022d2234(p, func_ov001_022d17bc);
    }
}
#pragma thumb reset
