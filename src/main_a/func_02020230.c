#include <nitro/types.h>

BOOL func_02020278(void *p);
void func_02020260(void *p);

BOOL func_02020230(void *p) {
    if (!func_02020278(p)) return FALSE;
    func_02020260(p);
    return TRUE;
}
