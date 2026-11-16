#include <ov000_d/mdl.h>

void func_020316f4(UnkStruct_MdlRender *r);

#pragma thumb on

void func_ov000_02356e2c(UnkStruct_Mdl *w) {
    switch (w->mode) {
    case 0:
        func_020316f4(&w->slot[0].r);
        break;
    case 1:
        func_020316f4(&w->slot[1].r);
        break;
    case 2:
        func_020316f4(&w->slot[0].r);
        func_020316f4(&w->slot[1].r);
        break;
    }
}

#pragma thumb reset
