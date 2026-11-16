// flags: -O1,p -proc arm946e -enum int -char signed -gccext,on -fp soft -interworking -sym on -gccinc -nolink -msgstyle gcc -w off -lang c99 -Cpp_exceptions off
#include <nitro/types.h>

extern u32 SND_IsFinishedCommandTag(u32 a);
extern BOOL SND_RecvCommandReply(u32 a);
extern void RequestCommandProc(void);

u32 SND_WaitForCommandProc(u32 a)
{
    u32 r;
    u32 block;
    if ((r = SND_IsFinishedCommandTag(a)) != 0) {
        return r;
    }
    block = 0;
    while (SND_RecvCommandReply(block) != 0) {
    }
    if ((r = SND_IsFinishedCommandTag(a)) != 0) {
        return r;
    }
    RequestCommandProc();
    if ((r = SND_IsFinishedCommandTag(a)) != 0) {
        return r;
    }
    block = 1;
    do {
        SND_RecvCommandReply(block);
    } while ((r = SND_IsFinishedCommandTag(a)) == 0);
    return r;
}
