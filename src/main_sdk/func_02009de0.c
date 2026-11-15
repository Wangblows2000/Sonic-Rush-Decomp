#include <nitro/types.h>

void func_02008ee8(void); void func_02008ed4(void);
void DC_StoreRange(void *p, u32 size);
typedef struct UnkStruct_02009de0 { u32 a; struct UnkStruct_02009de0 *next; } UnkStruct_02009de0;
typedef struct { u8 pad[0x18]; UnkStruct_02009de0 *list; } UnkStruct_02009de0_H;
void func_02009de0(UnkStruct_02009de0_H *h) {
    UnkStruct_02009de0 *p, *next;
    func_02008ee8();
    p = h->list;
    while (p != 0) { next = p->next; p->a = 0; p->next = 0; DC_StoreRange(p, 8); p = next; }
    func_02008ed4();
}
