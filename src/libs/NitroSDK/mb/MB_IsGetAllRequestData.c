#include <nitro/types.h>
#include <main_sdk2/blk2.h>

BOOL MB_IsGetAllRequestData(u16 aid)
{
    u16 i;
    for (i = 0; i < data_02080f48.count; i++) {
        if (!((1 << i) & data_02080f44->recvMask[aid - 1])) {
            return FALSE;
        }
    }
    return TRUE;
}
