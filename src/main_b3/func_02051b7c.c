#include <nitro/types.h>

typedef struct { u8 pad0[4]; s32 unk4; u8 pad8[0xc]; void *snd[5]; s32 unk28; } UnkWork_02051b7c;
extern UnkWork_02051b7c *data_022c465c;

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void func_020367b4(void *a, s32 b);

#pragma thumb on
void func_02051b7c(s32 n) {
    s32 id;

    if (data_022c465c->unk4 == n)
        return;
    switch (n) {
    case 0:
        id = 1;
        break;
    case 1:
        id = 2;
        break;
    case 2:
        id = 0x20;
        break;
    case 3:
        id = 0x21;
        break;
    case 4:
        id = 3;
        break;
    case 5:
        id = 4;
        break;
    default:
        return;
    }
    if (data_022c465c->unk28 == 0) {
        if (data_022c465c->snd[0] != NULL) {
            func_0201c070(&data_022c465c->snd[0], 0);
            func_0201bf1c(&data_022c465c->snd[0]);
        }
        func_020367b4(&data_022c465c->snd[0], id);
        data_022c465c->unk4 = n;
    } else {
        if (data_022c465c->snd[1] != NULL) {
            func_0201c070(&data_022c465c->snd[1], 0);
            func_0201bf1c(&data_022c465c->snd[1]);
        }
        func_020367b4(&data_022c465c->snd[1], id);
    }
}
#pragma thumb reset
