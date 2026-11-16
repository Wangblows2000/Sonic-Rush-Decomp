#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1c;
} UnkWork_022db9d8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db9d8 *work;
} UnkTask_022db9d8;

extern UnkTask_022db9d8 *data_022b4574;

void func_ov001_022d9eb4(UnkWork_022db9d8 *w);
void func_ov001_022da614(UnkWork_022db9d8 *w);
s32 func_ov001_022da558(void);
void func_ov001_022dab54(UnkWork_022db9d8 *w);
s32 func_02050f60(void);
void func_ov001_022d9e74(UnkWork_022db9d8 *w);
BOOL func_ov001_022da4a4(void);
BOOL func_ov001_022da48c(void);
BOOL func_ov001_022da52c(void);
BOOL func_ov001_022da4d4(void);
BOOL func_ov001_022da4ec(void);
void func_ov001_022da304(UnkWork_022db9d8 *w);
void func_02051f20(void);
void func_02051f90(void);
void func_02052104(s32 a);
void func_ov001_022da450(UnkWork_022db9d8 *w);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022db810(void);
void func_ov001_022db97c(void);
void func_ov001_022db8d0(void);

#pragma thumb on
void func_ov001_022db9d8(void) {
    UnkWork_022db9d8 *w = data_022b4574->work;
    s32 sel;
    BOOL cancel;
    BOOL empty;

    if (w->unk4 == 3) {
        func_ov001_022d9eb4(w);
    }
    func_ov001_022da614(w);
    sel = func_ov001_022da558();
    cancel = FALSE;
    empty = FALSE;
    if (w->unk1c == 0) {
        func_ov001_022dab54(w);
        w->unk14 = func_02050f60();
        if (w->unk14 > 8) {
            w->unk14 = 8;
        }
        if (w->unk10 < 0) {
            if (w->unk14 > 0) {
                w->unk10 = 0;
                w->unk4 = 0;
                func_ov001_022d9e74(w);
            }
        } else if (w->unk14 == 0) {
            w->unk10 = -1;
            w->unk4 = 0;
            empty = TRUE;
        } else if (w->unk10 >= w->unk14) {
            w->unk10 = w->unk14 - 1;
            w->unk4 = 0;
            func_ov001_022d9e74(w);
        } else if (func_ov001_022da4a4()) {
            if (w->unk10 > 0) {
                w->unk10--;
                w->unk4 = 0;
                func_ov001_022d9e74(w);
                func_02052104(4);
            }
        } else if (func_ov001_022da48c()) {
            if (w->unk10 < w->unk14 - 1) {
                w->unk10++;
                w->unk4 = 0;
                func_ov001_022d9e74(w);
                func_02052104(4);
            }
        } else if (func_ov001_022da52c()) {
            func_ov001_022da304(w);
            cancel = TRUE;
            func_02051f20();
        } else if (func_ov001_022da4d4()) {
            cancel = TRUE;
            func_02051f20();
        } else if (sel >= 0 && sel < w->unk14) {
            w->unk10 = sel;
            w->unk4 = 0;
            func_ov001_022d9e74(w);
            w->unk1c = 1;
            func_02051f90();
        } else if (func_ov001_022da4ec()) {
            w->unk4 = 0;
            w->unk1c = 1;
            func_02051f90();
        }
    }
    w->unk0++;
    w->unk4++;
    if (w->unk1c != 0 && w->unk4 >= 30) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        func_ov001_022d9eb4(w);
        func_ov001_022da450(w);
        func_02050078(5, 3, 1, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022db810;
        return;
    }
    if (cancel) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        func_ov001_022d9eb4(w);
        func_ov001_022da450(w);
        func_02050078(9, 3, 1, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022db97c;
        return;
    }
    if (empty) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        func_ov001_022d9eb4(w);
        data_022b4574->func = func_ov001_022db8d0;
    }
}
#pragma thumb reset
