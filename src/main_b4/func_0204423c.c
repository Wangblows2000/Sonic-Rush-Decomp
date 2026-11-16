#include <nitro/types.h>

typedef struct {
    u16 id;
    u8 dur;
    u8 flags;
} UnkSeq_0204423c;

typedef struct {
    void *res;
    u16 idx;
    u16 unk6;
    u16 flags;
    u16 unka;
    UnkSeq_0204423c *seq;
} UnkWork_0204423c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204423c *work;
} UnkTask_0204423c;

extern u16 data_022c4418[];
extern UnkTask_0204423c *data_022c4438[];

void func_0206d190(void *res, u16 id, u8 a, s32 b);
UnkTask_0204423c *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02044120(void);
void func_02044040(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *func_02033fbc(void *a, s32 b);

void func_0204423c(s32 idx, UnkSeq_0204423c *seq, void *res, void *path) {
    UnkTask_0204423c *t;
    UnkWork_0204423c *w;
    u16 i;

    if (data_022c4418[idx] != 0) {
        w = data_022c4438[idx]->work;
        data_022c4418[idx]++;
        for (i = 0;; i++) {
            if (w->seq[i].flags & 1)
                return;
            if (w->seq[i].dur == 0)
                break;
        }
        w->unk6 = 0;
        w->unka = w->seq->dur;
        func_0206d190(w->res, w->seq[w->unk6].id, w->idx, 0);
        func_0206d190(w->res, w->seq[w->unk6].id, w->idx, 1);
        return;
    }
    t = func_020372d0(func_02044120, func_02044040, 0, 0x3e00, 3, sizeof(UnkWork_0204423c));
    if (t == (UnkTask_0204423c *)func_02006abc(0))
        return;
    data_022c4438[idx] = t;
    data_022c4418[idx]++;
    w = t->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_0204423c));
    }
    w->seq = seq;
    w->res = res;
    w->idx = idx;
    w->unka = w->seq->dur;
    if (w->res == NULL) {
        w->res = func_02033fbc(path, 0);
        w->flags |= 1;
    }
}
