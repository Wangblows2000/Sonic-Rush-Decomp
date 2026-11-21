#include <nitro/types.h>

typedef struct UnkNode_0206cb84 {
    u16 used : 8;
    u16 prio : 2;
    u16 rest : 6;
    u8 pad2[2];
    void *data;
    struct UnkNode_0206cb84 *next;
} UnkNode_0206cb84;

extern UnkNode_0206cb84 data_022c4f1c[64];

void NetPacket_InitHeader(UnkNode_0206cb84 *n, s32 a, s32 b, u16 c);

UnkNode_0206cb84 *NetPacket_Queue(void *data, s32 a, s32 b, u16 c) {
    UnkNode_0206cb84 *e = NULL;
    UnkNode_0206cb84 *head = data_022c4f1c;
    UnkNode_0206cb84 *p;
    u16 i;

    for (i = 1; i < 64; i++) {
        if (head[i].used == 0) {
            e = &head[i];
            break;
        }
    }
    if (e == NULL)
        return NULL;
    NetPacket_InitHeader(e, a, b, c + 4);
    e->data = data;
    if (head->next == NULL) {
        head->next = e;
        e->next = NULL;
        return e;
    }
    for (p = head->next;; p = p->next) {
        if (p->prio < e->prio) {
            e->next = head->next;
            head->next = e;
            return e;
        }
        if (p->next == NULL) {
            p->next = e;
            e->next = NULL;
            return e;
        }
    }
}
