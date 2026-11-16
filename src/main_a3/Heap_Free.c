#include <nitro/types.h>

extern u8 data_0208ff00[0x20000];
extern u8 data_020aff00[0x1a0000];
extern u8 gMic[];
extern u8 data_027e0060[];

void func_02020990(void *heap, void *memBlock);

void Heap_Free(void *heap, u8 *p)
{
    if ((data_0208ff00 <= p && p < data_0208ff00 + sizeof(data_0208ff00))
        || (data_020aff00 <= p && p < gMic)
        || (data_027e0060 <= p && p < (u8 *)0x027e2060)) {
        func_02020990(heap, p);
    }
}
