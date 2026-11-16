#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    s32 unk4;
    s32 unk8;
    u8 unkc;
    u8 padd;
    u16 unke;
} UnkFade_0206199c;

void func_02061870(UnkFade_0206199c *f);
void func_02061940(UnkFade_0206199c *f);
BOOL func_020618a4(UnkFade_0206199c *f);

#pragma thumb on
BOOL func_0206199c(UnkFade_0206199c *f, u32 mode) {
    switch (mode) {
    case 0:
    case 2:
    default:
        func_02061870(f);
        f->state = NULL;
        break;
    case 1:
        if (f->unkc == 2) {
            f->unk4 = 0;
            f->unk8 = 0x3f00;
        } else {
            f->unk4 = 0x5000;
            f->unk8 = 0x3f00;
        }
        f->state = NULL;
        break;
    case 3:
        f->unke = 0x1e;
        f->state = (void (*)(void *))func_020618a4;
        func_02061870(f);
        break;
    }
    func_02061940(f);
    return TRUE;
}
#pragma thumb reset
