#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkWork_02050430;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02050430 *work;
} UnkTask_02050430;

extern UnkTask_02050430 *data_022c4638;
extern UnkTask_02050430 *data_022c4634;
extern UnkTask_02050430 *gTaskCurrent;
extern u32 gSystemFlags;
extern u16 data_0207ad9c;
void func_0204331c(s32 a);
void func_02042d34(s32 a);
void func_02042ab8(void);
void Task_Destroy(UnkTask_02050430 *t);

#pragma thumb on
void func_02050430(void) {
    u32 a;
    u32 b;
    if (data_022c4638 == NULL) {
        u32 v;
        u32 x;
        u32 y;
        UnkWork_02050430 *w;
        w = gTaskCurrent->work;
        v = w->unk0;
        x = v & 0xff;
        y = v >> 8;
        if (x > 1)
            x--;
        if (y > 1)
            y--;
        a = x;
        b = y;
        if (x == 1)
            x = 0;
        if (y == 1)
            y = 0;
        w->unk0 = x | (y << 8);
    } else {
        UnkWork_02050430 *w = data_022c4638->work;
        u32 v = w->unk8;
        u32 x = v & 0xff;
        u32 y = v >> 8;
        if (x > 1)
            x--;
        if (y > 1)
            y--;
        a = x;
        b = y;
        if (x == 1)
            x = 0;
        if (y == 1)
            y = 0;
        w->unk8 = x | (y << 8);
    }
    if (a == 1) {
        gSystemFlags &= ~0x200;
        switch (data_0207ad9c) {
        case 0:
            func_0204331c(0);
            break;
        case 1:
            func_02042d34(0);
            break;
        case 2:
            func_02042ab8();
            break;
        default:
            return;
        }
        a = 0;
        data_0207ad9c = 4;
    }
    if (a == 0 && b <= 1) {
        Task_Destroy(gTaskCurrent);
        data_022c4634 = NULL;
    }
}
#pragma thumb reset
