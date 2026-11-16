#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkStruct_02058350;
extern s16 gTalkDemoId;

void func_02059b04(u32 a);
u32 TalkDemo_GetFlags(s32 a);
UnkStruct_02058350 *func_02058350(void);

#pragma thumb on
void func_020605a4(void) {
    switch (gTalkDemoId) {
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
        if (TalkDemo_GetFlags(0) & 0x400)
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
