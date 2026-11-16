#include <nitro/types.h>

typedef struct UnkDtorChain {
    struct UnkDtorChain *next;
    void (*dtor)(void *);
    void *obj;
} UnkDtorChain;

typedef struct {
    u32 id;
    u32 ram_address;
    u32 ram_size;
    u32 bss_size;
} UnkOverlayInfo_0200c2b8;

extern UnkDtorChain *data_02087cac;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);

void FS_EndOverlay(UnkOverlayInfo_0200c2b8 *p_ovi)
{
    for (;;) {
        UnkDtorChain *head = NULL;
        UnkDtorChain *tail = NULL;
        u32 top = p_ovi->ram_address;
        u32 mid = top + p_ovi->ram_size;
        u32 bottom = mid + (u32)((s32)(p_ovi->ram_size + p_ovi->bss_size) - (s32)p_ovi->ram_size);
        u32 state = OS_DisableInterrupts();
        UnkDtorChain *prev = NULL;
        UnkDtorChain *base = data_02087cac;
        UnkDtorChain *p = base;
        while (p != NULL) {
            UnkDtorChain *next = p->next;
            u32 dtor = (u32)p->dtor;
            u32 obj = (u32)p->obj;
            if ((dtor >= top && dtor < mid) || (obj >= mid && obj < bottom)) {
                if (tail != NULL) {
                    tail->next = p;
                } else {
                    head = p;
                }
                if (base == p) {
                    base = data_02087cac = next;
                }
                p->next = NULL;
                tail = p;
                if (prev != NULL) {
                    prev->next = next;
                }
            } else {
                prev = p;
            }
            p = next;
        }
        OS_RestoreInterrupts(state);
        if (head == NULL) {
            break;
        }
        do {
            UnkDtorChain *next = head->next;
            if (head->dtor != NULL) {
                head->dtor(head->obj);
            }
            head = next;
        } while (head != NULL);
    }
}
