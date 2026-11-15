#include <nitro/types.h>

typedef struct UnkNode_02033754 UnkNode_02033754;
struct UnkNode_02033754 { UnkNode_02033754 *next; };
extern UnkNode_02033754 *data_0208f7e0;
extern UnkNode_02033754 *data_0208f7dc;
void func_02037e68(void *p);

void func_02033754(void) {
    UnkNode_02033754 *n = data_0208f7e0;
    while (n != NULL) {
        func_02037e68(n);
        n = data_0208f7e0->next;
        if (n == NULL) data_0208f7dc = NULL;
        data_0208f7e0 = n;
    }
}
