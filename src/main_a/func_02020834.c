#include <nitro/types.h>

void NNS_FndInitList(void *list, u16 offset);
void *func_020208c4(void *p);
void func_0202076c(void *list, void *obj);
extern u32 data_02084d28;
extern u8 data_02084d2c[];

typedef struct { u32 signature; u8 link[8]; u8 childList[0xC]; void *heapStart; void *heapEnd; u32 attribute; } UnkHeapHead_02020834;

void func_02020834(UnkHeapHead_02020834 *h, u32 sig, void *start, void *end, u16 opt) {
    h->signature = sig;
    h->heapStart = start;
    h->heapEnd = end;
    h->attribute = 0;
    h->attribute &= ~0xFF;
    h->attribute |= (u8)opt;
    NNS_FndInitList(h->childList, 4);
    if (data_02084d28 == 0) {
        NNS_FndInitList(data_02084d2c, 4);
        data_02084d28 = 1;
    }
    func_0202076c(func_020208c4(h), h);
}
