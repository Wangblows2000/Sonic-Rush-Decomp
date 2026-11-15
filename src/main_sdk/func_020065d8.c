#include <nitro/types.h>

typedef struct { u8 pad[4]; u32 x4; u32 x8; } UnkStruct_020065d8_T;
typedef struct { u8 pad[0x7c]; UnkStruct_020065d8_T *head; } UnkStruct_020065d8;
UnkStruct_020065d8_T *func_02006568(UnkStruct_020065d8 *q);
void func_02005bd4(UnkStruct_020065d8_T *t);
void func_020065d8(UnkStruct_020065d8 *q) {
    UnkStruct_020065d8_T *t;
    if (q->head == 0) return;
    do { t = func_02006568(q); t->x8 = 0; t->x4 = 0; func_02005bd4(t); } while (q->head != 0);
}
