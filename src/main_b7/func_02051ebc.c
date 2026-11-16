#include <nitro/types.h>

typedef struct {
    s32 kind;
} UnkState_02051ebc;

extern UnkState_02051ebc *data_022c465c;
extern void *data_02250458;
BOOL func_020521a0(void);
void func_020522f4(void);
void func_0201e7cc(s32 id, void *p);
void func_02052104(s32 a);

#pragma thumb on
void func_02051ebc(void) {
    if (func_020521a0() == 0)
        func_020522f4();
    if (data_022c465c->kind == 5) {
        func_0201e7cc(1, data_02250458);
        data_022c465c->kind = 0;
    }
    switch (data_022c465c->kind) {
    case 0:
        func_02052104(4);
        break;
    case 2:
        func_02052104(19);
        break;
    case 1:
        break;
    }
}
#pragma thumb reset
