#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 status;
} UnkJob_0204afc8;

typedef struct UnkState_0204afc8 UnkState_0204afc8;

typedef struct {
    const char *name;
    void *(*getMem)(char *path);
    void (*init)(UnkState_0204afc8 *s);
    void (*done)(UnkJob_0204afc8 *job, char *path);
} UnkEntry_0204afc8;

struct UnkState_0204afc8 {
    s32 state;
    s32 mode;
    char path[0x46];
    u16 step;
    u8 pad50[6];
    u16 unk56;
    u16 unk58;
    u8 pad5a[2];
    UnkJob_0204afc8 *job;
};

typedef struct {
    UnkEntry_0204afc8 *entries;
    u32 count;
} UnkArg_0204afc8;

extern UnkState_0204afc8 data_022c4490;

char *func_0202c518(char *dst, const char *src);
UnkJob_0204afc8 *func_02033f44(const char *path, void *mem);
s32 func_020491f4(s32 p0, s32 p1, s32 p2, s32 p3);
u32 func_02042978(void);
s32 func_0203c340(void);
void func_02033e3c(UnkJob_0204afc8 *n);
u32 func_0203e944(void);
u32 func_0203c27c(void);
u32 func_02042904(u16 v);

s32 func_0204afc8(UnkArg_0204afc8 *arg, s32 flag) {
    UnkState_0204afc8 *s = &data_022c4490;
    UnkEntry_0204afc8 *e;

    if (s->state == 1 || s->state == 2)
        return s->state;
    e = &arg->entries[s->step];
    switch (s->mode) {
    case 0:
        if (s->job == NULL) {
            func_0202c518(s->path, e->name);
            if (e->init != NULL)
                e->init(s);
            s->job = func_02033f44(s->path, e->getMem(s->path));
            s->unk58 = func_020491f4((u16)arg->count, s->step, 0, 0);
        } else if (s->job->status == 5) {
            if (func_02042978() == 2 && (func_0203c340() == 3 || func_0203c340() == 0)) {
                func_02033e3c(s->job);
                s->job = NULL;
                s->state = 2;
                return s->state;
            }
            if (e->done != NULL)
                e->done(s->job, s->path);
            func_02033e3c(s->job);
            s->job = NULL;
            s->step++;
            s->unk58 = func_020491f4((u16)arg->count, s->step, 0, 0);
        } else if (s->job->status == 3) {
            s->unk58 = func_020491f4((u16)arg->count, s->step, 0, 0);
        }
        break;
    case 1:
        if (s->job == NULL) {
            func_0202c518(s->path, e->name);
            if (e->init != NULL)
                e->init(s);
            s->job = func_02033f44(s->path, e->getMem(s->path));
        } else if (s->job->status == 5) {
            if ((func_0203e944() & ~1) == 0) {
                func_02033e3c(s->job);
                s->job = NULL;
                s->state = 2;
                return 2;
            }
            if (s->step < func_0203c27c()) {
                if (e->done != NULL)
                    e->done(s->job, s->path);
                func_02033e3c(s->job);
                s->job = NULL;
                s->step++;
            }
        }
        break;
    case 2: {
        u16 old = s->unk56;
        u16 n;
        s->unk56 = func_0203e944();
        if (func_02042904(old) > func_02042904(s->unk56)) {
            s->state = 2;
            return 2;
        }
        n = func_0203c27c();
        if (flag == 0)
            n -= 2;
        if (s->step < n)
            s->step++;
        s->unk58 = func_020491f4((u16)arg->count, s->step, 0, 0);
        break;
    }
    }
    if (arg->count <= s->step)
        s->state = 1;
    return s->state;
}
