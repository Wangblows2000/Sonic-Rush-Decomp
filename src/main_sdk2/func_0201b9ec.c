#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    void *unk1c;
} UnkHeap_0201b9ec;

typedef struct {
    void *handle;
    UnkHeap_0201b9ec *heap;
    u8 pad8[0x2c - 8];
    u8 status;
    u8 pad2d;
    u8 pad2e;
    u8 pauseReq;
    u32 seqId;
    u8 pad34[8];
    u8 playerNo;
} UnkSndPlayer_0201b9ec;

void func_02008e74(u32 player);
void func_02008d50(u32 player, u32 mask, void *x);
void func_0201b89c(UnkSndPlayer_0201b9ec *p);
u32 func_020091ac(void);

void func_0201b9ec(UnkSndPlayer_0201b9ec *p)
{
    UnkHeap_0201b9ec *heap = p->heap;
    func_02008e74(p->playerNo);
    if (heap->unk1c != NULL) {
        func_02008d50(p->playerNo, 0xffff, heap->unk1c);
    }
    func_0201b89c(p);
    p->seqId = func_020091ac();
    p->pauseReq = TRUE;
    p->status = 1;
}
