#include <nitro/types.h>

typedef struct UnkNode_02022a10 UnkNode_02022a10;
typedef void (*UnkFunc_02022a10)(void *, UnkNode_02022a10 *, u32);
struct UnkNode_02022a10 { u8 unk0[0xC]; UnkFunc_02022a10 func; UnkNode_02022a10 *next; u8 unk14[6]; u16 unk1A[1]; };

BOOL func_02022a10(void *ctx, UnkNode_02022a10 *n, u32 idx) {
    BOOL r = FALSE;
    do {
        u16 v = n->unk1A[idx];
        if (v & 0x100) {
            n->func(ctx, n, v & 0xFF);
            r = TRUE;
        }
        n = n->next;
    } while (n != NULL);
    return r;
}
