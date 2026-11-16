#include <nitro/types.h>

extern BOOL IsCommandAvailable(void);
extern u32 AllocCommand(void);
extern s32 SND_CountWaitingCommand(void);
extern BOOL SND_RecvCommandReply(u32 a);
extern void SND_FlushCommand(u32 a);
extern void RequestCommandProc(void);

u32 SND_AllocCommand(u32 a)
{
    u32 r;
    if (!IsCommandAvailable()) {
        return 0;
    }
    if ((r = AllocCommand()) != 0) {
        return r;
    }
    if (!(a & 1)) {
        return 0;
    }
    if (SND_CountWaitingCommand() > 0) {
        while (SND_RecvCommandReply(0) != 0) {
        }
        if ((r = AllocCommand()) != 0) {
            return r;
        }
    } else {
        SND_FlushCommand(1);
    }
    RequestCommandProc();
    do {
        SND_RecvCommandReply(1);
    } while ((r = AllocCommand()) == 0);
    return r;
}
