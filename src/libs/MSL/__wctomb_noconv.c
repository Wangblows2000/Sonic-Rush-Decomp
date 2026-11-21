#include <nitro/types.h>

BOOL __wctomb_noconv(u8 *p, u8 v) {
    if (p == NULL) return FALSE;
    *p = v;
    return TRUE;
}
