#include <nitro/types.h>

typedef struct { void *prevObject; void *nextObject; } NNSFndLink;
typedef struct { void *headObject; void *tailObject; u16 numObjects; u16 offset; } NNSFndList;
void func_0202076c(NNSFndList *list, void *object);
void func_02020708(NNSFndList *list, void *object);

#define GET_LINK(list, obj) ((NNSFndLink *)((u8 *)(obj) + (list)->offset))

void func_02020688(NNSFndList *list, void *target, void *object) {
    if (target == NULL) {
        func_0202076c(list, object);
    } else if (target == list->headObject) {
        func_02020708(list, object);
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
