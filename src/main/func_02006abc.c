#include <nitro/types.h>

/* Reads word `i` of the table at 0x027FFDA0 (shared main-RAM work area).
 * Matched (CW 1.2). Likely NitroSDK; name unknown. */
u32 func_02006abc(s32 i) {
    return ((u32 *)0x027FFDA0)[i];
}
