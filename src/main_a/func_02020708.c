#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;
void SetFirstObject(NNSFndList *list, void *object);

#define GET_LINK(list, obj) ((NNSFndLink *)((u8 *)(obj) + (list)->offset))

void func_02020708(NNSFndList *list, void *object) {
    if (list->headObject == NULL) {
        SetFirstObject(list, object);
    } else {
        NNSFndLink *link = GET_LINK(list, object);
        link->prevObject = NULL;
        link->nextObject = list->headObject;
        GET_LINK(list, list->headObject)->prevObject = object;
        list->headObject = object;
        list->numObjects++;
    }
}
