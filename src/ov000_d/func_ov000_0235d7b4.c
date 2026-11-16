#include <ov000_d/mdl2.h>

#pragma thumb on

void func_ov000_0235d7b4(UnkStruct_Mdl2 *w, s32 sel) {
    s32 f0;
    void *a1;
    void *a0;
    s32 f1;

    switch (sel) {
    case 0:
        f0 = 0x37;
        f1 = 1;
        a0 = w->anm8F8;
        a1 = w->anm8FC;
        w->angB = 0;
        w->flagB |= 4;
        break;
    case 1:
        f0 = 6;
        f1 = 6;
        a0 = w->anm900;
        a1 = w->anm904;
        w->angB = 0;
        w->flagB &= ~4;
        break;
    case 2:
        f0 = 7;
        f1 = 7;
        a0 = w->anm900;
        a1 = w->anm904;
        w->angB = 0;
        w->flagB &= ~4;
        break;
    default:
        f0 = 0x37;
        f1 = 1;
        a0 = w->anm8F8;
        a1 = w->anm8FC;
        w->angB = 0;
        w->flagB |= 4;
        break;
    }
    func_02031a68(&w->rB, 0, a0, f0, NULL);
    func_02031a68(&w->rB, 4, a1, f1, NULL);
}

#pragma thumb reset
