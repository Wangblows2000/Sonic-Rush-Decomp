#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u16 unk1C;
    u16 unk1E[0xb];
    u16 unk34;
    u16 unk36[0xb];
} UnkWork_02050764;
typedef struct { u8 pad0[0x10]; UnkWork_02050764 *work; } UnkTask_02050764;
extern UnkTask_02050764 *data_022c4638;
extern UnkTask_02050764 *gTaskCurrent;
extern u16 data_022c1950;
extern u16 data_022c4644[];
extern s32 data_022c4640;

s32 func_02042958(void);
void Task_Destroy(UnkTask_02050764 *t);
void func_02050b58(u16 a);
BOOL func_020502e4(UnkWork_02050764 *w);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02042810(u16 *buf, s32 size, s32 a);
void func_020429d0(void);
u16 WH_GetCurrentAid(void);
void func_020516b4(void);

#pragma thumb on
void func_02050764(void) {
    UnkTask_02050764 *t = data_022c4638;
    UnkWork_02050764 *w = t->work;
    s32 st;
    s32 r;

    if (t == NULL)
        st = 7;
    else
        st = w->unkC;
    if (st < 6) {
        r = func_02042958();
        if (w->unk18) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        if (r == 7 || st == 0 || w->unk14) {
            func_02050b58(data_022c1950);
            return;
        }
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            w->unk8 = 0;
            break;
        case 2:
            if (func_020502e4(w)) {
                u16 buf[0xc];
                MIi_CpuCopy16(w->unk1E, buf, 0x16);
                buf[0xb] = 2;
                func_02042810(buf, 0x18, 1);
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (r != 2) {
                func_02050b58(data_022c1950);
            } else if (w->unk10) {
                func_020429d0();
                w->unk1C = WH_GetCurrentAid();
                w->unkC = 4;
                w->unk8 = 0;
            }
            break;
        case 4:
            if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                MIi_CpuCopy16(w->unk36, data_022c4644, 0x16);
                data_022c4640 = 1;
            } else if (r != 2) {
                func_02050b58(data_022c1950);
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
