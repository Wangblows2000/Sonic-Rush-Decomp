#pragma opt_propagation off
#pragma opt_strength_reduction off
#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0x60]; } UnkStruct_02339c2c;

extern UnkStruct_02339c2c data_ov000_0236aad8[];
extern UnkStruct_02339c2c data_ov000_0236ab3c;
void func_ov000_02338c28(void *a);
void func_ov000_02342834(u16 i);
void func_ov000_02339bdc(void *a, s32 i);
void func_ov000_02339364(void *a, s32 i);
void func_ov000_023394b4(void *a, s32 i);
void func_ov000_02339264(void *a, s32 i);
void func_ov000_023391dc(void *a, s32 i);
void func_ov000_023395a4(void *a, s32 i);
void func_ov000_02339154(void *a, s32 i);
void func_ov000_02339094(void *a, s32 i);
void func_ov000_0233905c(void *a, s32 i);

void func_ov000_02339c2c(void *a) {
    UnkStruct_02339c2c *base;
    UnkStruct_02339c2c *q;
    UnkStruct_02339c2c *p;
    s32 i;
    if ((data_ov000_0236aad8[2].flags & 0x100) == 0)
        func_ov000_02338c28(a);
    base = data_ov000_0236aad8;
    if (base[2].flags & 0x200)
        return;
    if (base[2].flags & 1) {
        if ((base[0].flags & 0x140) == 0) {
            func_ov000_02339bdc(a, 0);
            func_ov000_023391dc(a, 0);
        } else {
            func_ov000_02339364(a, 0);
        }
        if ((base[0].flags & 0x280) == 0) {
            func_ov000_023395a4(a, 0);
            func_ov000_02339154(a, 0);
        } else {
            func_ov000_02339264(a, 0);
        }
        q = &data_ov000_0236ab3c;
        if ((q->flags & 0x140) == 0)
            func_ov000_02339094(a, 1);
        if ((q->flags & 0x280) == 0)
            func_ov000_0233905c(a, 1);
        return;
    }
    i = 0;
    while (i < 2) {
        p = &base[i];
        func_ov000_02342834(i);
        if ((base[i].flags & 0x140) == 0) func_ov000_02339bdc(a, i); else func_ov000_02339364(a, i);
        if ((p->flags & 0x280) == 0) func_ov000_023394b4(a, i); else func_ov000_02339264(a, i);
        i++;
    }
}
