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

typedef enum { WF_PCM8, WF_PCM16 } UnkWaveFmt_0201cacc;
typedef enum { CF_PCM16, CF_PCM8 } UnkCapFmt_0201cacc;

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

BOOL NNSi_SndCaptureStart(int type, void *buffer0, void *buffer1, u32 bufLen, int format, int input, int output, BOOL loopFlag, int sampleRate, int volume, int pan0, int pan1, u32 interval, void *callback, void *arg)
{
    UnkWaveFmt_0201cacc wave_format;
    UnkCapFmt_0201cacc capture_format;
    u32 chBitMask = 0;
    u32 playChBitMask = 0;
    u32 capBitMask = 0;
    int alarmNo = -1;
    unsigned int samples;
    int timer;
    u32 alarmTimer;
    u32 alarmOffset;
    BOOL is8bit;
    UnkCapture_0201cacc *cap = &data_02083e10;

    DC_FlushRange(buffer0, bufLen);
    DC_FlushRange(buffer1, bufLen);

    is8bit = format == 1 ? TRUE : FALSE;

    timer = 0xffb0ff / sampleRate;

    if (callback != NULL) {
        samples = bufLen;
        if (!is8bit)
            samples >>= 1;
        timer = ((timer + 16) & ~0x1f);
        alarmTimer = (timer >> 5) * (samples / interval);
        alarmOffset = 32;
        if (!is8bit)
            alarmOffset >>= 1;
        alarmOffset *= (timer >> 5);
    }

    wave_format = is8bit ? WF_PCM8 : WF_PCM16;
    capture_format = is8bit ? CF_PCM8 : CF_PCM16;

    chBitMask |= (1 << 1) | (1 << 3);
    capBitMask |= (1 << 0) | (1 << 1);

    if (type != 2)
        playChBitMask = chBitMask;

    if (callback != NULL) {
        alarmNo = NNS_SndAllocAlarm();
        if (alarmNo < 0)
            return FALSE;
    }

    if (!NNS_SndLockCapture(capBitMask)) {
        if (alarmNo >= 0)
            NNS_SndFreeAlarm(alarmNo);
        return FALSE;
    }

    if (!NNS_SndLockChannel(chBitMask)) {
        if (alarmNo >= 0)
            NNS_SndFreeAlarm(alarmNo);
        NNS_SndUnlockCapture(capBitMask);
        return FALSE;
    }

    SND_SetupChannelPcm(1, wave_format, buffer0, loopFlag ? 1 : 2, 0, (int)(bufLen >> 2), volume, 0, timer, pan0);
    SND_SetupCapture(0, capture_format, buffer0, bufLen >> 2, loopFlag, input, output);
    SND_SetupChannelPcm(3, wave_format, buffer1, loopFlag ? 1 : 2, 0, (int)(bufLen >> 2), volume, 0, timer, pan1);
    SND_SetupCapture(1, capture_format, buffer1, bufLen >> 2, loopFlag, input, output);

    if (alarmNo >= 0)
        SND_SetupAlarm(alarmNo, alarmTimer + alarmOffset, alarmTimer, AlarmCallback_capture, cap);

    if (type == 1)
        SND_SetOutputSelector(1, 2, 1, 1);

    SND_StartTimer(playChBitMask, capBitMask, alarmNo >= 0 ? (u32)(1 << alarmNo) : 0, 0);

    cap->active = TRUE;
    cap->type = type;
    cap->chBits = chBitMask;
    cap->ch = playChBitMask;
    cap->cap = capBitMask;
    cap->alarm = alarmNo;
    cap->format = format;
    cap->buf0 = buffer0;
    cap->buf1 = buffer1;
    cap->bufSize = bufLen;
    cap->numBlocks = bufLen / interval;
    cap->curBlock = 0;
    cap->interval = interval;
    cap->callback = callback;
    cap->arg = arg;
    cap->volume = volume;
    NNSi_SndFaderInit(cap->fader);
    NNSi_SndFaderSet(cap->fader, volume << 8, 1);
    cap->unk4C = FALSE;
    return TRUE;
}
