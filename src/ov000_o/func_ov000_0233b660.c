#include <nitro/types.h>

typedef enum {
    DISP_SELECT_SUB_MAIN = 0,
    DISP_SELECT_MAIN_SUB = 1
} DispSelect_0233b660;

static inline DispSelect_0233b660 GetDispSelect_0233b660(void) {
    return (DispSelect_0233b660)((*(vu16 *)0x4000304 & 0x8000) >> 15);
}

BOOL func_ov000_0233b660(void) {
    DispSelect_0233b660 d = GetDispSelect_0233b660();

    if (d == DISP_SELECT_MAIN_SUB) {
        return FALSE;
    }
    if (d == DISP_SELECT_SUB_MAIN) {
        return TRUE;
    }
    return TRUE;
}
