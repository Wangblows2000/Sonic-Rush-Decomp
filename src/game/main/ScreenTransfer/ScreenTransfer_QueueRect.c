#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 unk4[1];
} UnkScr_02035d04;

typedef struct UnkJob_02035d04 {
    struct UnkJob_02035d04 *next;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u32 unkC;
    u16 unk10;
    u16 unk12;
    UnkScr_02035d04 *src;
    u32 unk18;
    u32 unk1c;
} UnkJob_02035d04;

typedef struct {
    u32 v[2];
} UnkPair_02035d04;

typedef struct {
    u16 a;
    u16 b;
} UnkWH_02035d04;

extern UnkPair_02035d04 data_0207803c;
extern u16 data_0207316c[];
extern UnkWH_02035d04 data_0207319c[];

void ScreenTransfer_QueueRectRaw(void *data, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h);
UnkJob_02035d04 *ScreenTransfer_AllocJob(void);
void *OS_GetArenaLo(u32 arg);

void ScreenTransfer_QueueRect(UnkScr_02035d04 *src, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h) {
    u32 idx;
    UnkPair_02035d04 tbl;
    UnkJob_02035d04 *job;
    u16 k;
    if (!(src->unk0 & 0xf0)) {
        ScreenTransfer_QueueRectRaw(src->unk4, x, y, a, flip, mode, c, d, e, f, w, h);
        return;
    }
    if (mode >= 0 && mode < 12) {
        idx = 0;
    } else {
        idx = 1;
    }
    tbl = data_0207803c;
    job = ScreenTransfer_AllocJob();
    k = data_0207316c[mode];
    if (OS_GetArenaLo(0) == job) {
        return;
    }
    job->unk4 = 1;
    job->unk6 = k * data_0207319c[mode].a;
    job->unk8 = k * (f * data_0207319c[mode].a + e);
    job->unkC = tbl.v[idx] + ((c << 16) + (d << 11));
    job->src = src;
    job->unk18 = k * a;
    job->unk1c = k * (a * y + x);
    job->unk10 = k * w;
    job->unk12 = h;
}
