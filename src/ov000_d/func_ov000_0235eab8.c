#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235eab8_Sync;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_0235eab8_Task;

extern UnkStruct_0235eab8_Task *data_022b4574;
extern s32 data_02087ccc;
extern u32 data_02087cb8;

UnkStruct_0235eab8_Sync *func_020514e4(void);
UnkStruct_0235eab8_Sync *func_02051474(void);
UnkStruct_0235eab8_Sync *func_02051404(void);
BOOL func_ov000_0235cbd8(void);
BOOL func_020513a8(void);
void *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_ov000_0235cadc(void);
void func_020521b4(void);
void func_0204b2ec(void);
void func_020522f4(void);
void func_02052168(s32 a);
void func_ov000_0235ea40(void);

#pragma thumb on

void func_ov000_0235eab8(void) {
    BOOL go = FALSE;
    UnkStruct_0235eab8_Sync *a = func_020514e4();
    UnkStruct_0235eab8_Sync *b = func_02051474();
    UnkStruct_0235eab8_Sync *c = func_02051404();

    if (func_ov000_0235cbd8()) {
        if (a->unk3 == 0 && b->unk3 == 0) {
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
            go = TRUE;
        }
    } else {
        if (b->unk3 == 0) {
            a->unk3 = go;
        }
        if (a->unk3 == 0) {
            if (b->unk3 == 1) {
                a->unk3 = 1;
            }
        } else if (a->unk3 == 1) {
            if (b->unk3 == 2) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2) {
            if (b->unk3 == 3) {
                go = TRUE;
            }
        }
    }
    if (go) {
        func_020372d0(func_ov000_0235cadc, NULL, 0, 0x2000, 0x1000, 0);
        if (func_020513a8()) {
            func_020521b4();
            func_0204b2ec();
        }
        func_020522f4();
        func_02052168(3);
        data_02087ccc = 1;
        data_02087cb8 |= 0x10;
        data_022b4574->unk8 = func_ov000_0235ea40;
    }
}

#pragma thumb reset
