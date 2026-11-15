#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    void *unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x18];
    u16 unk34;
    u16 unk36;
} UnkWork_02050510;
typedef struct { u8 pad0[0x10]; UnkWork_02050510 *work; } UnkTask_02050510;
typedef struct { u8 pad0[0x14]; u16 unk14; } UnkStruct_022c4644;
extern UnkTask_02050510 *data_022c4638;
extern UnkStruct_022c4644 data_022c4644;
extern s32 data_022c4640;

s32 func_02042958(void);
s32 func_02042938(void);
s32 func_02042998(void);
void func_02054dbc(void);
void func_02051838(void);
void func_02050e0c(void *a);
void func_020429d0(void);
u16 func_02050d60(s32 a, void *b, s32 c);
void func_020516b4(void);

#pragma thumb on
void func_02050510(void) {
    UnkTask_02050510 *t = data_022c4638;
    UnkWork_02050510 *w = t->work;
    s32 st;
    s32 r;

    if (t == NULL)
        st = 8;
    else
        st = w->unkC;
    if (st < 7) {
        r = func_02042958();
        if (r == 7 || st == 0 || w->unk18) {
            if (func_02042938() == 3) {
                func_02054dbc();
            } else {
                void *p = w->unk10;
                func_02051838();
                func_02050e0c(p);
            }
            return;
        }
        w->unk36 = func_02042998() - 1;
        if (w->unk36 > w->unk34)
            w->unk36 = 0;
        if (func_02042938() == 2)
            w->unk36 = 0;
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            w->unk8 = 0;
            break;
        case 2:
            if (w->unk36 != 0 && r == 2) {
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (w->unk36 == 0) {
                w->unkC = 2;
                w->unk8 = 0;
            } else if (w->unk14) {
                func_020429d0();
                w->unkC = 4;
                w->unk8 = 0;
                data_022c4644.unk14 = func_02050d60(0, &data_022c4644, 10);
                data_022c4640 = 1;
            }
            break;
        case 4:
            if (w->unk36 == 0) {
                w->unkC = 2;
                w->unk8 = 0;
            } else if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                w->unk14 = 0;
                w->unk18 = 0;
                func_020516b4();
                return;
            }
            break;
        case 5:
        case 6:
        case 7:
            return;
        }
        w->unk8++;
        w->unk14 = 0;
        w->unk18 = 0;
    }
}
#pragma thumb reset
