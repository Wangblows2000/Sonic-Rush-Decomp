#include <nitro/types.h>

typedef struct {
    u32 a;
    u32 b;
} UnkStruct_0200bb20;

BOOL func_0200bc44(UnkStruct_0200bb20 *out);
BOOL func_0200bb70(void *p, UnkStruct_0200bb20 v);

BOOL func_0200bb20(void *p)
{
    UnkStruct_0200bb20 v;
    if (func_0200bc44(&v) && func_0200bb70(p, v)) {
        return TRUE;
    }
    return FALSE;
}
