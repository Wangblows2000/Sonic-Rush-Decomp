#include <nitro/types.h>

typedef struct {
    u32 active;
    int type;
    int format;
    void *buf0;
    void *buf1;
    u32 bufSize;
    u32 numBlocks;
    u32 curBlock;
    int chBits;
    int ch;
    int cap;
    int alarm;
    u32 interval;
    void *callback;
    void *arg;
    u8 fader[0x10];
    u32 unk4C;
    int volume;
} UnkCapture_0201cacc;

extern UnkCapture_0201cacc data_02083e10;

void DC_FlushRange(const void *addr, u32 size);
int NNS_SndAllocAlarm(void);
void NNS_SndFreeAlarm(int alarm);
BOOL NNS_SndLockCapture(u32 mask);
void NNS_SndUnlockCapture(u32 mask);
BOOL NNS_SndLockChannel(u32 mask);
void SND_SetupChannelPcm(int ch, int format, const void *data, int loop, int loopStart, int loopLen, int volume, int shift, int timer, int pan);
void SND_SetupCapture(int cap, int format, void *buf, u32 len, BOOL loop, int in, int out);
void SND_SetupAlarm(int alarm, u32 tick, u32 period, void *cb, void *arg);
void SND_SetOutputSelector(int a, int b, int c, int d);
void SND_StartTimer(int ch, int cap, u32 alarmBits, int d);
void AlarmCallback_capture(void *arg);
void NNSi_SndFaderInit(void *fader);
void NNSi_SndFaderSet(void *fader, s32 v, u32 c);

BOOL NNSi_SndCaptureStart(int type, void *buf0, void *buf1, u32 bufSize, int format, int in, int out, BOOL loop, int sampleRate, int volume, int pan0, int pan1, u32 interval, void *callback, void *arg)
{
    UnkCapture_0201cacc *cap;
    int timer;
    BOOL isPcm8;
    int alarm;
    u32 extra;
    int ch;
    u32 period;
    u32 units;
    int fmtCh;
    int fmtCap;
    u32 len;

    cap = &data_02083e10;
    ch = 0;
    alarm = -1;
    DC_FlushRange(buf0, bufSize);
    DC_FlushRange(buf1, bufSize);
    isPcm8 = (format == 1);
    timer = 0xffb0ff / sampleRate;
    if (callback != NULL) {
        u32 n;
        timer = (timer + 0x10) & ~0x1f;
        units = timer >> 5;
        n = bufSize;
        if (!isPcm8)
            n = bufSize >> 1;
        period = units * (n / interval);
        extra = 0x20;
        if (!isPcm8)
            extra >>= 1;
        extra = extra * units;
    }
    if (isPcm8)
        fmtCh = 0;
    else
        fmtCh = 1;
    if (isPcm8)
        fmtCap = 1;
    else
        fmtCap = 0;
    if (type != 2)
        ch = 10;
    if (callback != NULL) {
        alarm = NNS_SndAllocAlarm();
        if (alarm < 0)
            return FALSE;
    }
    if (!NNS_SndLockCapture(3)) {
        if (alarm >= 0)
            NNS_SndFreeAlarm(alarm);
        return FALSE;
    }
    if (!NNS_SndLockChannel(10)) {
        if (alarm >= 0)
            NNS_SndFreeAlarm(alarm);
        NNS_SndUnlockCapture(3);
        return FALSE;
    }
    len = bufSize >> 2;
    SND_SetupChannelPcm(1, fmtCh, buf0, loop ? 1 : 2, 0, len, volume, 0, timer, pan0);
    SND_SetupCapture(0, fmtCap, buf0, len, loop, in, out);
    SND_SetupChannelPcm(3, fmtCh, buf1, loop ? 1 : 2, 0, len, volume, 0, timer, pan1);
    SND_SetupCapture(1, fmtCap, buf1, len, loop, in, out);
    if (alarm >= 0)
        SND_SetupAlarm(alarm, period + extra, period, AlarmCallback_capture, cap);
    if (type == 1)
        SND_SetOutputSelector(1, 2, 1, 1);
    SND_StartTimer(ch, 3, alarm >= 0 ? 1 << alarm : 0, 0);
    cap->active = 1;
    cap->type = type;
    cap->chBits = 10;
    cap->ch = ch;
    cap->cap = 3;
    cap->alarm = alarm;
    cap->format = format;
    cap->buf0 = buf0;
    cap->buf1 = buf1;
    cap->bufSize = bufSize;
    cap->numBlocks = bufSize / interval;
    cap->curBlock = 0;
    cap->interval = interval;
    cap->callback = callback;
    cap->arg = arg;
    cap->volume = volume;
    NNSi_SndFaderInit(cap->fader);
    NNSi_SndFaderSet(cap->fader, volume << 8, 1);
    cap->unk4C = 0;
    return TRUE;
}
