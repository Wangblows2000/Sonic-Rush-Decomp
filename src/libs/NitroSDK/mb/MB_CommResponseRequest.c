#include <nitro/types.h>
#include <main_sdk2/wh.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL IsChildAidValid(u16 aid);

BOOL MB_CommResponseRequest(u16 aid, u32 req)
{
    u16 val;
    u32 state;
    u32 e = OS_DisableInterrupts();
    switch (req) {
    case 0:
        state = 10;
        val = 4;
        break;
    case 1:
        state = 10;
        val = 3;
        break;
    case 2:
        state = 14;
        val = 2;
        break;
    case 3:
        state = 7;
        val = 5;
        break;
    default:
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (data_02080f1c != NULL && IsChildAidValid(aid) && state == data_02080f1c->unk14e8[aid - 1]) {
        data_02080f1c->unk1754[aid - 1] = val;
        OS_RestoreInterrupts(e);
        return TRUE;
    }
    OS_RestoreInterrupts(e);
    return FALSE;
}
