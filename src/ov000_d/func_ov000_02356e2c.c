#include <ov000_d/mdl.h>

void Model_Animate(UnkStruct_MdlRender *r);

#pragma thumb on

void func_ov000_02356e2c(UnkStruct_Mdl *w) {
    switch (w->mode) {
    case 0:
        Model_Animate(&w->slot[0].r);
        break;
    case 1:
        Model_Animate(&w->slot[1].r);
        break;
    case 2:
        Model_Animate(&w->slot[0].r);
        Model_Animate(&w->slot[1].r);
        break;
    }
}

#pragma thumb reset
