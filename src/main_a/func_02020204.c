#include <nitro/types.h>

void func_0201fc60(void *p);

void func_02020204(void **pp) {
    void *p = *pp;
    if (p == NULL) return;
    func_0201fc60(p);
}
