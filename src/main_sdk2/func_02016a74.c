#include <nitro/types.h>
#include <main_sdk2/wh.h>

BOOL func_020156fc(u16 aid);

BOOL func_02016a74(u16 aid)
{
    if (data_02080f1c != NULL && func_020156fc(aid) && data_02080f1c->unk14e8[aid - 1] == 7) {
        return TRUE;
    }
    return FALSE;
}
