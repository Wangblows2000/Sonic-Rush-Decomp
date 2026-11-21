#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;

void SetFirstObject(NNSFndList *list, void *object) {
    NNSFndLink *link = (NNSFndLink *)((u8 *)object + list->offset);
    link->nextObject = NULL;
    link->prevObject = NULL;
    list->headObject = object;
    list->tailObject = object;
    list->numObjects++;
}
