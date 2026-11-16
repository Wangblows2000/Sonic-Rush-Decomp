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

extern UnkState_0204afc8 gStageLoader;

char *strcpy(char *dst, const char *src);
UnkJob_0204afc8 *FileRequest_Create(const char *path, void *mem);
s32 Math_CalcPercent(s32 p0, s32 p1, s32 p2, s32 p3);
u32 WirelessMgr_GetMode(void);
s32 WFS_GetStatus(void);
void FileRequest_Destroy(UnkJob_0204afc8 *n);
u32 WH_GetBitmap(void);
u32 WFS_GetSentFileCount(void);
u32 Math_CountBits16(u16 v);

s32 StageLoader_ProcessFileList(UnkArg_0204afc8 *arg, s32 flag) {
    UnkState_0204afc8 *s = &gStageLoader;
    UnkEntry_0204afc8 *e;

    if (s->state == 1 || s->state == 2)
        return s->state;
    e = &arg->entries[s->step];
    switch (s->mode) {
    case 0:
        if (s->job == NULL) {
            strcpy(s->path, e->name);
            if (e->init != NULL)
                e->init(s);
            s->job = FileRequest_Create(s->path, e->getMem(s->path));
            s->unk58 = Math_CalcPercent((u16)arg->count, s->step, 0, 0);
        } else if (s->job->status == 5) {
            if (WirelessMgr_GetMode() == 2 && (WFS_GetStatus() == 3 || WFS_GetStatus() == 0)) {
                FileRequest_Destroy(s->job);
                s->job = NULL;
                s->state = 2;
                return s->state;
            }
            if (e->done != NULL)
                e->done(s->job, s->path);
            FileRequest_Destroy(s->job);
            s->job = NULL;
            s->step++;
            s->unk58 = Math_CalcPercent((u16)arg->count, s->step, 0, 0);
        } else if (s->job->status == 3) {
            s->unk58 = Math_CalcPercent((u16)arg->count, s->step, 0, 0);
        }
        break;
    case 1:
        if (s->job == NULL) {
            strcpy(s->path, e->name);
            if (e->init != NULL)
                e->init(s);
            s->job = FileRequest_Create(s->path, e->getMem(s->path));
        } else if (s->job->status == 5) {
            if ((WH_GetBitmap() & ~1) == 0) {
                FileRequest_Destroy(s->job);
                s->job = NULL;
                s->state = 2;
                return 2;
            }
            if (s->step < WFS_GetSentFileCount()) {
                if (e->done != NULL)
                    e->done(s->job, s->path);
                FileRequest_Destroy(s->job);
                s->job = NULL;
                s->step++;
            }
        }
        break;
    case 2: {
        u16 old = s->unk56;
        u16 n;
        s->unk56 = WH_GetBitmap();
        if (Math_CountBits16(old) > Math_CountBits16(s->unk56)) {
            s->state = 2;
            return 2;
        }
        n = WFS_GetSentFileCount();
        if (flag == 0)
            n -= 2;
        if (s->step < n)
            s->step++;
        s->unk58 = Math_CalcPercent((u16)arg->count, s->step, 0, 0);
        break;
    }
    }
    if (arg->count <= s->step)
        s->state = 1;
    return s->state;
}
