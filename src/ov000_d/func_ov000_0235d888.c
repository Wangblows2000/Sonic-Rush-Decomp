#include <ov000_d/mdl2.h>

#pragma thumb on

void func_ov000_0235d888(UnkStruct_Mdl2 *w, s32 sel) {
    s32 f0;
    void *a1;
    void *a0;
    s32 f1;

    switch (sel) {
    case 0:
        f0 = 0;
        f1 = 1;
        a0 = w->anm8F8;
        a1 = w->anm8FC;
        w->angA = 0;
        w->flagA |= 4;
        break;
    case 1:
        f0 = 2;
        f1 = 2;
        a0 = w->anm900;
        a1 = w->anm904;
        w->angA = 0;
        w->flagA &= ~4;
        break;
    case 2:
        f0 = 3;
        f1 = 3;
        a0 = w->anm900;
        a1 = w->anm904;
        w->angA = 0;
        w->flagA &= ~4;
        break;
    default:
        f0 = 0;
        f1 = 1;
        a0 = w->anm8F8;
        a1 = w->anm8FC;
        w->angA = 0;
        w->flagA |= 4;
        break;
    }
    func_02031a68(&w->rA, 0, a0, f0, NULL);
    func_02031a68(&w->rA, 4, a1, f1, NULL);
}

#pragma thumb reset
