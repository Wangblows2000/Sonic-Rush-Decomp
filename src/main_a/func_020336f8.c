#include <nitro/types.h>

typedef struct UnkNode_020336f8 UnkNode_020336f8;
struct UnkNode_020336f8 { UnkNode_020336f8 *next; };
extern UnkNode_020336f8 *data_0208f7e0;
extern UnkNode_020336f8 *data_0208f7dc;
UnkNode_020336f8 *func_02037eb0(void);
UnkNode_020336f8 *func_02006abc(u32 a);

UnkNode_020336f8 *func_020336f8(void) {
    UnkNode_020336f8 *n = func_02037eb0();
    if (n != func_02006abc(0)) {
        if (data_0208f7e0 == NULL) {
            data_0208f7e0 = n;
        } else {
            data_0208f7dc->next = n;
        }
        data_0208f7dc = n;
        n->next = NULL;
    }
    return n;
}
