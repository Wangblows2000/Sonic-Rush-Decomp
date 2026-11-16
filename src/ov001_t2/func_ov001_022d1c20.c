#include <nitro/types.h>

typedef struct {
    s32 mode;
    u8 pad4[0xbb0 - 4];
    s32 unkbb0;
    s32 unkbb4;
} UnkStruct_022d1c20;

void func_ov001_022d2018(UnkStruct_022d1c20 *p);
void func_ov001_022d0128(s32 i);
void func_ov001_022d2234(UnkStruct_022d1c20 *p, void (*fn)(UnkStruct_022d1c20 *));
void func_ov001_022d1bec(UnkStruct_022d1c20 *p);
void func_ov001_022d1acc(UnkStruct_022d1c20 *p);
void func_ov001_022d1a04(UnkStruct_022d1c20 *p);

#pragma thumb on
void func_ov001_022d1c20(UnkStruct_022d1c20 *p) {
    s32 i;

    p->unkbb0 = 0;
    p->unkbb4 = 0;
    func_ov001_022d2018(p);
    for (i = 0; i < 4; i++) {
        func_ov001_022d0128(i);
    }
    switch (p->mode) {
    case 0:
        func_ov001_022d2234(p, func_ov001_022d1bec);
        break;
    case 1:
        func_ov001_022d2234(p, func_ov001_022d1acc);
        break;
    case 2:
        func_ov001_022d2234(p, func_ov001_022d1a04);
        break;
    }
}
#pragma thumb reset
