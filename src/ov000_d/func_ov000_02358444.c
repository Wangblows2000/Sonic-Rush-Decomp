#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_02358444_Sync;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
} UnkStruct_02358444;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02358444 *work;
} UnkStruct_02358444_Task;

extern UnkStruct_02358444_Task *data_022b4574;

UnkStruct_02358444_Sync *func_020514e4(void);
UnkStruct_02358444_Sync *func_02051474(void);
UnkStruct_02358444_Sync *func_02051404(void);
void func_ov000_02356694(s32 x, s32 y, s32 a, s32 b);
void func_02052168(s32 a);
void func_ov000_02359c0c(UnkStruct_02358444_Task *t);

#pragma thumb on

void func_ov000_02358444(void) {
    UnkStruct_02358444 *w = data_022b4574->work;
    UnkStruct_02358444_Sync *a = func_020514e4();
    UnkStruct_02358444_Sync *b = func_02051474();
    UnkStruct_02358444_Sync *c = func_02051404();

    if (w->unkC == 1) {
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
            func_ov000_02356694(0x10, 0, 0x10, 0);
            func_02052168(2);
            func_ov000_02359c0c(data_022b4574);
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
        } else if (a->unk3 == 2 && b->unk3 == 3) {
            func_ov000_02356694(0x10, 0, 0x10, 0);
            func_02052168(2);
            func_ov000_02359c0c(data_022b4574);
        }
    }
}

#pragma thumb reset
