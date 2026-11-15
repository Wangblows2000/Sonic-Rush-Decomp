#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; s32 unk8; } UnkNode_02034114;
UnkNode_02034114 *func_02033d58(void);
void func_02033b98(UnkNode_02034114 *n);
void func_02033b44(UnkNode_02034114 *n);

void func_02034114(void) {
    BOOL cont = TRUE;
    do {
        UnkNode_02034114 *n = func_02033d58();
        if (n == NULL) return;
        switch (n->unk8) {
        case 1:
            func_02033b98(n);
            cont = FALSE;
            break;
        case 3:
            func_02033b44(n);
            if (n->unk8 != 5) cont = FALSE;
            break;
        }
    } while (cont);
}
