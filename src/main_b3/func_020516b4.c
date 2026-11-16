#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; } UnkWork_020516b4;
typedef struct { u8 pad0[0x10]; UnkWork_020516b4 *work; } UnkTask_020516b4;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern UnkTask_020516b4 *data_022c4638;
extern UnkTask_020516b4 *gTaskCurrent;
extern UnkStruct_0207ad9c data_0207ad9c;
extern u32 gSystemFlags;
extern u8 data_022c44f8[];
extern u8 data_022c4500[];

void func_02051628(s32 a);
BOOL func_02042958(void);
void func_02043204(void *p);
void func_0204331c(s32 a);
void func_02042cec(void *p);
void func_02042d34(s32 a);
void func_02042a70(void *p);
void func_02042ab8(void);
void Task_Destroy(UnkTask_020516b4 *t);
void func_020508a8(s32 a, s32 b);
void func_0204319c(void *p);
void func_02042c84(void *p);
void func_02042a20(void *p, s32 a);

#pragma thumb on
void func_020516b4(void) {
    if (data_022c4638 != NULL) {
        UnkWork_020516b4 *w = data_022c4638->work;
        data_0207ad9c.unk0 = w->unk0;
        data_0207ad9c.unk2 = w->unk4;
    }
    func_02051628(0);
    switch (data_0207ad9c.unk2) {
    case 0:
    case 1:
        switch (data_0207ad9c.unk0) {
        case 0:
            gSystemFlags |= 0x200;
            if (func_02042958())
                func_02043204(data_022c44f8);
            func_0204331c(1);
            break;
        case 1:
            if (func_02042958())
                func_02042cec(data_022c44f8);
            func_02042d34(1);
            break;
        case 2:
            if (func_02042958())
                func_02042a70(data_022c44f8);
            func_02042ab8();
            data_0207ad9c.unk0 = 4;
            Task_Destroy(gTaskCurrent);
            data_022c4638 = NULL;
            return;
        default:
            return;
        }
        func_020508a8(0x18, 0x20);
        break;
    case 2:
    case 3:
        switch (data_0207ad9c.unk0) {
        case 0:
            gSystemFlags |= 0x200;
            if (func_02042958())
                func_0204319c(data_022c4500);
            func_0204331c(1);
            break;
        case 1:
            if (func_02042958())
                func_02042c84(data_022c4500);
            func_02042d34(1);
            break;
        case 2:
            if (func_02042958())
                func_02042a20(data_022c4500, 1);
            func_02042ab8();
            data_0207ad9c.unk0 = 4;
            Task_Destroy(gTaskCurrent);
            data_022c4638 = NULL;
            return;
        default:
            return;
        }
        func_020508a8(0, 0x20);
        break;
    }
}
#pragma thumb reset
