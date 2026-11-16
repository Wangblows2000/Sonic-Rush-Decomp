#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235e23c_Sync;

typedef struct {
    s32 unk0;
    u8 pad4[0x1C - 4];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern void *gTaskCurrent;

UnkStruct_0235e23c_Sync *func_020514e4(void);
UnkStruct_0235e23c_Sync *func_02051474(void);
UnkStruct_0235e23c_Sync *func_02051404(void);
BOOL func_ov000_0235cbd8(void);
BOOL func_020513a8(void);
void func_0204b314(s32 a, u8 b, u8 c);
void Task_Destroy(void *t);
void func_0204d6d0(s32 a);

#pragma thumb on

void func_ov000_0235e23c(void) {
    UnkStruct_0235e23c_Sync *a = func_020514e4();
    UnkStruct_0235e23c_Sync *b = func_02051474();
    UnkStruct_0235e23c_Sync *c = func_02051404();

    if (func_ov000_0235cbd8()) {
        if (a->unk3 == 0 && b->unk3 == 0) {
            if (func_020513a8()) {
                func_0204b314(data_022c4560.unk0, data_022c4560.unk1C, data_022c4560.unk1D);
            }
            a->unk3 = 1;
        } else if (a->unk3 == 1) {
            if (b->unk3 == 1) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2) {
            if (b->unk3 == 2) {
                a->unk3 = 3;
            }
        } else if (c->unk3 >= 3) {
            Task_Destroy(gTaskCurrent);
            func_0204d6d0(0);
        }
    } else {
        if (b->unk3 == 0) {
            a->unk3 = 0;
        }
        if (a->unk3 == 0 && b->unk3 == 1) {
            a->unk3 = 1;
        }
        if (a->unk3 == 1) {
            if (b->unk3 == 2) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2 && b->unk3 >= 3) {
            Task_Destroy(gTaskCurrent);
            func_0204d6d0(0);
        }
    }
}

#pragma thumb reset
