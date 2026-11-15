#include <nitro/types.h>

typedef struct UnkNode_02035368 UnkNode_02035368;
struct UnkNode_02035368 { UnkNode_02035368 *next; };
extern UnkNode_02035368 *data_02250438;
extern UnkNode_02035368 *data_02250434;
void func_02037e68(void *p);

void func_02035368(void) {
    UnkNode_02035368 *n = data_02250438;
    while (n != NULL) {
        func_02037e68(n);
        n = data_02250438->next;
        if (n == NULL) data_02250434 = NULL;
        data_02250438 = n;
    }
}
