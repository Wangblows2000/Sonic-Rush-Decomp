#include <nitro/types.h>

typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;

void NNS_FndInitList(NNSFndList *list, u16 offset) {
    list->headObject = NULL;
    list->tailObject = NULL;
    list->numObjects = 0;
    list->offset = offset;
}
