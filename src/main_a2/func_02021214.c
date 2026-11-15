#include <nitro/types.h>

typedef struct { void *head; void *tail; void *state; } UnkFrmHeap_02021214;
typedef struct {
    u32 signature;
    u8 link[8];
    u8 childList[0xC];
    void *heapStart;
    void *heapEnd;
    u32 attribute;
    UnkFrmHeap_02021214 frm;
} UnkHeap_02021214;

void func_02020834(UnkHeap_02021214 *heap, u32 signature, void *start, void *end, u16 opt);

UnkHeap_02021214 *func_02021214(UnkHeap_02021214 *heap, void *end, u16 opt) {
    UnkFrmHeap_02021214 *fh = &heap->frm;
    func_02020834(heap, 0x46524D48, fh + 1, end, opt);
    heap->frm.head = heap->heapStart;
    fh->tail = heap->heapEnd;
    fh->state = NULL;
    return heap;
}
