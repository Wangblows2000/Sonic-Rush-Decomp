#include <nitro/types.h>

void StrmCallback_stream(void *p, u32 v);

void AlarmCallback_stream(void *p) {
    StrmCallback_stream(p, 1);
}
