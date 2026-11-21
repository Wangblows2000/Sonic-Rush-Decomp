#include <nitro/types.h>

typedef struct UnkStruct_022de14c { void (*fn)(void *a, struct UnkStruct_022de14c *p); } UnkStruct_022de14c;

void TransformDemo_RunFadeState(void *a, UnkStruct_022de14c *p) {
    if (p->fn != NULL) {
        p->fn(a, p);
    }
}
