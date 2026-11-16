#include <nitro/types.h>

u32 ExtraAct_GetCompressionType(u32 *p) {
    return *p & 0xf0;
}
