#include <nitro/types.h>

typedef struct UnkNode_02014288 {
    u32 id;
    u8 pad4[0x24];
    struct UnkNode_02014288 *next; /* 0x28 */
    u8 *data;                      /* 0x2c */
} UnkNode_02014288;

UnkNode_02014288 *func_0201558c(void);
s32 func_0201566c(void);

u8 *func_02014288(u32 id, s32 idx)
{
    UnkNode_02014288 *n = func_0201558c();
    if (id < 1000) {
        u32 i;
        i = 0;
    loop:
        if (n != NULL) {
            if (i == id) {
                return (u8 *)n + idx * func_0201566c();
            }
            i++;
            n = n->next;
            goto loop;
        }
        return NULL;
    }
    while (n != NULL) {
        if (n->id == id) {
            u8 *data = n->data;
            return data + idx * func_0201566c();
        }
        n = n->next;
    }
    return NULL;
}
