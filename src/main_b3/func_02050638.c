#include <nitro/types.h>

typedef struct { u16 v[0xb]; } UnkName_02050638;
typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x18];
    u16 unk34;
    u16 unk36;
    UnkName_02050638 names[1]; /* 0x38 */
} UnkWork_02050638;
typedef struct { u8 pad0[0x10]; UnkWork_02050638 *work; } UnkTask_02050638;
extern UnkTask_02050638 *data_022c4638;
extern UnkTask_02050638 *data_022b4574;
extern u16 data_022c1950;
extern u16 data_022c4644[];
extern s32 data_022c4640;

s32 func_02042958(void);
void func_020371c8(UnkTask_02050638 *t);
void func_02050a7c(u16 a);
void func_02043278(u16 a);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_020516b4(void);

#pragma thumb on
void func_02050638(void) {
    UnkTask_02050638 *t = data_022c4638;
    UnkWork_02050638 *w = t->work;
    s32 st;
    s32 r;

    if (t == NULL)
        st = 7;
    else
        st = w->unkC;
    if (st < 6) {
        r = func_02042958();
        if (w->unk18) {
            func_020371c8(data_022b4574);
            return;
        }
        if (r == 7 || st == 0 || w->unk14) {
            func_02050a7c(data_022c1950);
            return;
        }
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            break;
        case 2:
            if (w->unk34) {
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (w->unk34 == 0) {
                func_02050a7c(data_022c1950);
                return;
            }
            if (w->unk10) {
                func_02043278(w->unk36);
                w->unkC = 4;
                w->unk8 = 0;
            }
            break;
        case 4:
            if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                MIi_CpuCopy16(&w->names[w->unk36], data_022c4644, 0x16);
                data_022c4640 = 1;
            } else if (r == 7) {
                func_02050a7c(data_022c1950);
                return;
            }
            break;
        case 5:
            w->unkC = 6;
            w->unk8 = 0;
            w->unk10 = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            func_020516b4();
            return;
        case 6:
            break;
        }
        w->unk8++;
        w->unk10 = 0;
        w->unk14 = 0;
        w->unk18 = 0;
    }
}
#pragma thumb reset
