#include <nitro/types.h>
#include <main_sdk2/wh.h>

BOOL IsChildAidValid(u16 aid);

BOOL MB_CommIsBootable(u16 aid)
{
    if (data_02080f1c != NULL && IsChildAidValid(aid) && data_02080f1c->unk14e8[aid - 1] == 7) {
        return TRUE;
    }
    return FALSE;
}
