#include <nitro/types.h>

extern s32 data_022c1980;
extern void (*data_022c197c)(const char *s, ...);
extern char data_020786a4[];
extern char data_020786c0[];
extern s32 data_022c196c;

void func_020405e0(s32 a);
void func_0203dda4(void);
BOOL func_0203ed7c(void);
void func_0203c350(void);
BOOL func_0203ed38(void);
BOOL func_0203fcf4(void);
BOOL func_0203fc44(void);

void func_0203dc28(void) {
    if (data_022c1980 == 1) {
        if (data_022c197c != NULL)
            data_022c197c(data_020786a4);
        return;
    }
    if (data_022c197c != NULL)
        data_022c197c(data_020786c0, data_022c1980);
    if (data_022c1980 != 6 && data_022c1980 != 5 && data_022c1980 != 4) {
        func_020405e0(3);
        func_0203dda4();
        return;
    }
    func_020405e0(3);
    switch (data_022c196c) {
    case 3:
        if (!func_0203ed7c())
            func_0203dda4();
        break;
    case 7:
        func_0203c350();
    case 1:
    case 5:
        if (!func_0203ed38())
            func_0203dda4();
        break;
    case 2:
        if (!func_0203fcf4())
            func_0203dda4();
        break;
    case 6:
        func_0203c350();
    case 0:
    case 4:
        if (!func_0203fc44())
            func_0203dda4();
        break;
    }
}
