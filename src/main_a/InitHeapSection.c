#include <nitro/types.h>

void NNS_FndInitList(void *list, u16 offset);

void InitHeapSection(void *list) {
    NNS_FndInitList(list, 0);
}
