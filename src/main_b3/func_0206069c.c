#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkStruct_02058350;
extern s16 data_022c4da0;

u32 func_02059b1c(void);
void func_02060e54(s32 a, s32 b);
UnkStruct_02058350 *func_02058350(void);

#pragma thumb on
void func_0206069c(void) {
    switch (data_022c4da0) {
    case 0xa:
        if (func_02059b1c() & 0x20000)
            func_02060e54(0, 0x100);
        break;
    case 0xb:
        if (func_02059b1c() & 0x40000)
            func_02060e54(0, 0x100);
        break;
    case 0xf:
        if (func_02059b1c() & 0x80000)
            func_02060e54(0, 0x100);
        break;
    case 0x10:
        if (func_02059b1c() & 0x100000)
            func_02060e54(0, 0x100);
        break;
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x38:
    case 0x39:
        if (func_02059b1c() & 0x1000)
            func_02060e54(0, 0x100);
        break;
    case 0xe:
        if (func_02059b1c() & 0x1000) {
            func_02060e54(0, 0x100);
            func_02058350()->flags |= 0x100000;
        }
        break;
    }
}
#pragma thumb reset
