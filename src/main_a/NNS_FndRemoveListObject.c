#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;

#define GET_LINK(list, obj) ((NNSFndLink *)((u8 *)(obj) + (list)->offset))

void NNS_FndRemoveListObject(NNSFndList *list, void *object) {
    NNSFndLink *link = GET_LINK(list, object);
    if (link->prevObject == NULL) {
        list->headObject = link->nextObject;
    } else {
        GET_LINK(list, link->prevObject)->nextObject = link->nextObject;
    }
    if (link->nextObject == NULL) {
        list->tailObject = link->prevObject;
    } else {
        GET_LINK(list, link->nextObject)->prevObject = link->prevObject;
    }
    link->prevObject = NULL;
    link->nextObject = NULL;
    list->numObjects--;
}
