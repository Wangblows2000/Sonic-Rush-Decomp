#include <nitro/types.h>

typedef struct UnkNode_02035cb4 UnkNode_02035cb4;
struct UnkNode_02035cb4 { UnkNode_02035cb4 *next; };
extern UnkNode_02035cb4 *data_02250440;
extern UnkNode_02035cb4 *data_0225043c;
void func_02037e68(void *p);

void func_02035cb4(void) {
    UnkNode_02035cb4 *n = data_02250440;
    while (n != NULL) {
        func_02037e68(n);
        n = data_02250440->next;
        if (n == NULL) data_0225043c = NULL;
        data_02250440 = n;
    }
}
