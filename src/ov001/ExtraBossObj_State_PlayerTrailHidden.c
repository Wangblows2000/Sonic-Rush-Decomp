#include <nitro/types.h>

typedef struct { u8 pad[0x18]; void *next; } UnkStruct_022ee2b0_n;
typedef struct { s32 speed; u8 pad[0x2a0]; UnkStruct_022ee2b0_n *cur; s32 v2a8; } UnkStruct_022ee2b0_a;
typedef struct { u8 pad[4]; s32 kind; } UnkStruct_022ee2b0_b;
void ExtraBoss_UpdatePlayerTrailNode(void *a, void *p);
void ExtraBossObj_State_PlayerTrailGrow(void);

void ExtraBossObj_State_PlayerTrailHidden(u8 *p) {
    UnkStruct_022ee2b0_a *a = *(UnkStruct_022ee2b0_a **)(p + 0x1dc);
    UnkStruct_022ee2b0_b *b = *(UnkStruct_022ee2b0_b **)(*(u8 **)(p + 0x7c) + 0x1dc);
    void *n;
    n = a->cur->next;
    a->cur = n;
    ExtraBoss_UpdatePlayerTrailNode(n, p);
    if (b->kind == 5 || b->kind == 6 || b->kind == 0xc) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_PlayerTrailGrow;
    a->v2a8 = 0;
    *(u32 *)(p + 0x14) &= ~0x20000;
    switch (b->kind) {
    case 3:
    case 4:
        a->speed = 0x10000;
        break;
    default:
        a->speed = 0xc000;
        break;
    }
}
