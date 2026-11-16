#include <ov000_c/stageobj.h>

extern void *data_ov000_0236ac60[];
extern u8 data_ov000_02366b44[];

void func_ov000_02343228(void *p, s32 a);
void func_ov000_023431c0(void *p, s32 a);
void func_ov000_0234307c(void *p, s32 a);
void func_ov000_02342fb4(void *p);
void func_ov000_02342f74(void *p);
void func_ov000_02342f34(void *p);
void func_ov000_02342f14(void *p);
void func_ov000_02342ef4(void *p);
void func_ov000_02342e9c(void *p);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void func_ov000_022fa9e8(void *a, void *p, s32 b);

void func_ov000_0230da90(void *a, void *p, s32 k) {
    switch (k) {
    case 0:
    default:
        func_ov000_02343228(p, data_ov000_02366b44[(s16)(Rand() & 7)]);
        break;
    case 1:
        func_ov000_02343228(p, 5);
        break;
    case 3:
        func_ov000_0234307c(p, 0x12c0);
        break;
    case 4:
        func_ov000_0234307c(p, 0x640);
        break;
    case 2:
        func_ov000_023431c0(p, 1);
        break;
    case 5:
        func_ov000_02342fb4(p);
        break;
    case 6:
        func_ov000_02342f74(p);
        break;
    case 7:
        func_ov000_02342f34(p);
        break;
    case 8:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        func_ov000_022fa9e8(a, p, 7);
        func_ov000_02342f14(data_ov000_0236ac60[1]);
        break;
    case 9:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        func_ov000_022fa9e8(a, p, 8);
        func_ov000_02342ef4(data_ov000_0236ac60[1]);
        break;
    case 10:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        func_ov000_0234307c(p, 0x12c0);
        func_ov000_02342e9c(data_ov000_0236ac60[1]);
        func_ov000_022fa9e8(a, p, 9);
        break;
    case 11:
        Sound_PlaySeqArcEx(0, 0, 0xdf);
        func_ov000_022fa9e8(a, p, 10);
        break;
    }
}
