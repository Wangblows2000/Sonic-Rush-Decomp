#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;

void *NNS_FndGetNextListObject(NNSFndList *list, void *object) {
    if (object == NULL) {
        return list->headObject;
    }
    return ((NNSFndLink *)((u8 *)object + list->offset))->nextObject;
}
