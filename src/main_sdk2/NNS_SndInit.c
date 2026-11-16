#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    void *arg;
    void *next;
} UnkPMCallback;

extern u32 data_0208104c;
extern UnkPMCallback data_02081050;
extern UnkPMCallback data_0208105c;
extern s8 data_02081040;
extern u32 data_02081044;

void SND_Init(void);
void BeginSleep(void *arg);
void EndSleep(void *arg);
void PM_PrependPreSleepCallback(UnkPMCallback *cb);
void PM_AppendPostSleepCallback(UnkPMCallback *cb);
void NNSi_SndInitResourceMgr(void);
void NNSi_SndCaptureInit(void);
void NNSi_SndPlayerInit(void);

static inline void func_0201b3d4_SetCb(UnkPMCallback *cb, void (*func)(void *), void *arg)
{
    cb->func = func;
    cb->arg = arg;
}

void NNS_SndInit(void)
{
    if (data_0208104c != 0) {
        return;
    }
    data_0208104c = 1;
    SND_Init();
    func_0201b3d4_SetCb(&data_02081050, BeginSleep, NULL);
    func_0201b3d4_SetCb(&data_0208105c, EndSleep, NULL);
    PM_PrependPreSleepCallback(&data_02081050);
    PM_AppendPostSleepCallback(&data_0208105c);
    NNSi_SndInitResourceMgr();
    NNSi_SndCaptureInit();
    NNSi_SndPlayerInit();
    data_02081040 = -1;
    data_02081044 = 1;
}
