#include <ov000_d/sprobj.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkStruct_0235e79c_Pad;

extern UnkStruct_0235e79c_Pad data_022503e0;

BOOL func_ov000_0235cccc(void);
BOOL func_ov000_0235cca0(void);
BOOL func_ov000_0235cc74(void);
void func_ov000_0235ce2c(UnkStruct_SprObj *w, s32 sel);
u8 *func_02051474(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_02052104(s32 a);
void func_ov000_0235ce18(UnkStruct_SprObj *w);
void func_ov000_0235e738(void);

#pragma thumb on

void func_ov000_0235e79c(void) {
    UnkStruct_SprObj *w = data_022b4574->work;
    BOOL done;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    func_ov000_0235def0(w, w->unk20, 0x1000);
    func_ov000_0235ddec(w, FALSE);
    done = FALSE;
    if (func_ov000_0235cccc()) {
        w->unk8 = done;
        done = TRUE;
    } else if (func_ov000_0235cca0()) {
        done = TRUE;
        w->unk8 = done;
    } else if (func_ov000_0235cc74()) {
        w->unk8 = 2;
        done = TRUE;
    } else if (data_022503e0.trg & 1) {
        done = TRUE;
    } else if (data_022503e0.rep & 0x40) {
        w->unk8--;
        if (w->unk8 < 0) {
            w->unk8 = 2;
        }
        w->unk4 = 0;
    } else if (data_022503e0.rep & 0x80) {
        w->unk8++;
        if (w->unk8 >= 3) {
            w->unk8 = done;
        }
        w->unk4 = 0;
    }
    func_ov000_0235ce2c(w, w->unk8);
    w->unk0++;
    w->unk4++;
    if (done) {
        MI_CpuCopy8(func_02051474() + 6, w->unkC14, 6);
        func_02052104(7);
        w->unk0 = 0;
        func_ov000_0235ce18(w);
        data_022b4574->unk8 = func_ov000_0235e738;
    }
}

#pragma thumb reset
