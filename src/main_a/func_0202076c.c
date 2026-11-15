#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;
void SetFirstObject(NNSFndList *list, void *object);

#define GET_LINK(list, obj) ((NNSFndLink *)((u8 *)(obj) + (list)->offset))

void func_0202076c(NNSFndList *list, void *object) {
    if (list->headObject == NULL) {
        SetFirstObject(list, object);
    } else {
        NNSFndLink *link = GET_LINK(list, object);
        link->prevObject = list->tailObject;
        link->nextObject = NULL;
        GET_LINK(list, list->tailObject)->nextObject = object;
        list->tailObject = object;
        list->numObjects++;
    }
}
