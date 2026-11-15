#include <nitro/types.h>

typedef struct UnkNode_02035c58 UnkNode_02035c58;
struct UnkNode_02035c58 { UnkNode_02035c58 *next; };
extern UnkNode_02035c58 *data_02250440;
extern UnkNode_02035c58 *data_0225043c;
UnkNode_02035c58 *func_02037eb0(void);
UnkNode_02035c58 *func_02006abc(u32 a);

UnkNode_02035c58 *func_02035c58(void) {
    UnkNode_02035c58 *n = func_02037eb0();
    if (n != func_02006abc(0)) {
        if (data_02250440 == NULL) {
            data_02250440 = n;
        } else {
            data_0225043c->next = n;
        }
        data_0225043c = n;
        n->next = NULL;
    }
    return n;
}
