#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;
void NNS_FndAppendListObject(NNSFndList *list, void *object);
void NNS_FndPrependListObject(NNSFndList *list, void *object);

#define GET_LINK(list, obj) ((NNSFndLink *)((u8 *)(obj) + (list)->offset))

void NNS_FndInsertListObject(NNSFndList *list, void *target, void *object) {
    if (target == NULL) {
        NNS_FndAppendListObject(list, object);
    } else if (target == list->headObject) {
        NNS_FndPrependListObject(list, object);
    } else {
        NNSFndLink *link = GET_LINK(list, object);
        void *prev = GET_LINK(list, target)->prevObject;
        NNSFndLink *prevLink = GET_LINK(list, prev);
        link->prevObject = prev;
        link->nextObject = target;
        prevLink->nextObject = object;
        GET_LINK(list, target)->prevObject = object;
        list->numObjects++;
    }
}
