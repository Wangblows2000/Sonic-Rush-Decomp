#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkStruct_02058350;
extern s16 data_022c4da0;

void func_02059b04(u32 a);
u32 func_02060e70(s32 a);
UnkStruct_02058350 *func_02058350(void);

#pragma thumb on
void func_020605a4(void) {
    switch (data_022c4da0) {
    case 0xa:
        func_02059b04(0x20000);
        break;
    case 0xb:
        func_02059b04(0x40000);
        break;
    case 0xf:
        func_02059b04(0x80000);
        break;
    case 0x10:
        func_02059b04(0x100000);
        break;
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x38:
    case 0x39:
        if (func_02060e70(0) & 0x400)
            func_02058350()->flags |= 0x200000;
        break;
    case 0xe: {
        UnkStruct_02058350 *p = func_02058350();
        p->flags &= ~0x100000;
        p->flags &= ~0x800000;
        break;
    }
    }
}
#pragma thumb reset
