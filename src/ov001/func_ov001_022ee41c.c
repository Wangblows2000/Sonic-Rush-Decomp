#include <nitro/types.h>

typedef struct { u8 pad[0x18]; void *next; } UnkStruct_022ee2b0_n;
typedef struct { s32 speed; u8 pad[0x2a0]; UnkStruct_022ee2b0_n *cur; s32 v2a8; } UnkStruct_022ee2b0_a;
typedef struct { u8 pad[4]; s32 kind; } UnkStruct_022ee2b0_b;
void func_ov001_022ee100(void *a, void *p);
void func_ov001_022ee354(void);

void func_ov001_022ee41c(u8 *p) {
    UnkStruct_022ee2b0_a *a = *(UnkStruct_022ee2b0_a **)(p + 0x1dc);
    UnkStruct_022ee2b0_b *b = *(UnkStruct_022ee2b0_b **)(*(u8 **)(p + 0x7c) + 0x1dc);
    void *n;
    n = a->cur->next;
    a->cur = n;
    func_ov001_022ee100(n, p);
    switch (b->kind) {
    case 5:
    case 6:
    case 12:
        *(void (**)(void))(p + 0x74) = func_ov001_022ee354;
        a->speed = 0xa000;
        break;
    case 3:
    case 4:
        a->speed = 0x10000;
        break;
    default:
        a->speed = 0xc000;
        break;
    }
}
