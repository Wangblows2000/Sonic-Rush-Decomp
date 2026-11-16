#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_Hit;

void func_0206d8bc(UnkStruct_Hit *a, UnkStruct_Hit *b);
void func_02069818(void *w, s32 a);
void func_ov000_023548d8(UnkStruct_StageObj *p, s16 a, s32 b);
void func_ov000_02355da0(UnkStruct_StageObj *p, UnkStruct_StageObj *w);

static inline s32 Abs(s32 x) { return x < 0 ? -x : x; }

void func_ov000_02301d2c(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w;
    s32 t;
    s32 d;
    UnkStruct_StageObj *p;
    s32 v;

    w = a->unk18;
    p = w->unk1BC;

    if (p != NULL) {
    if (p->unk0 == 1 && p == b->unk18 && w->unk1D6 == 0) {
        {
            s32 dx = p->unk10 - w->unk10;
            if (dx < 0)
                dx = -dx;
            d = dx >> 8;
        }
        if (d < 0x10) {
            t = 10;
            func_02069818(w, 3);
            v = -0x500;
            w->unk8 = 0;
        } else if (d < 0x20) {
            t = 15;
            func_02069818(w, 2);
            v = -0x580;
            w->unk8 = 1;
        } else {
            t = 20;
            func_02069818(w, 1);
            v = -0x600;
            w->unk8 = 2;
        }
        w->unkC = t;
        func_ov000_023548d8(p, v - d * 0x18, t);
        func_ov000_02355da0(p, w);
    } else {
        func_0206d8bc(a, b);
    }
    } else {
        func_0206d8bc(a, b);
    }
}
