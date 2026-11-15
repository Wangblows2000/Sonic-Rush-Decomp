#include <nitro/types.h>

typedef struct UnkNode_0203530c UnkNode_0203530c;
struct UnkNode_0203530c { UnkNode_0203530c *next; };
extern UnkNode_0203530c *data_02250438;
extern UnkNode_0203530c *data_02250434;
UnkNode_0203530c *func_02037eb0(void);
UnkNode_0203530c *func_02006abc(u32 a);

UnkNode_0203530c *func_0203530c(void) {
    UnkNode_0203530c *n = func_02037eb0();
    if (n != func_02006abc(0)) {
        if (data_02250438 == NULL) {
            data_02250438 = n;
        } else {
            data_02250434->next = n;
        }
        data_02250434 = n;
        n->next = NULL;
    }
    return n;
}
