#include <nitro/types.h>
#include <NitroSDK/wh.h>

static inline BOOL func_0201a088_a(void)
{
    BOOL r = FALSE;
    if (data_02080f18->unk528 == 1 && data_02080f18->unk50c == 0) {
        r = TRUE;
    }
    return r;
}

static inline BOOL func_0201a088_b(void)
{
    BOOL r = FALSE;
    if (func_0201a088_a() && data_02080f18->unk526 == 0) {
        r = TRUE;
    }
    return r;
}

BOOL MBi_IsSendEnabled(void)
{
    BOOL r = FALSE;
    if (func_0201a088_b() && data_02080f18->unk52a != 0) {
        r = TRUE;
    }
    return r;
}
