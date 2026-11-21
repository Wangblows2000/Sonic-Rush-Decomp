#ifndef MAIN_A2_EXPHEAP_H
#define MAIN_A2_EXPHEAP_H
#include <nitro/types.h>

typedef struct UnkMBlock UnkMBlock;
struct UnkMBlock {
    u16 signature;
    u16 attribute;
    u32 blockSize;
    UnkMBlock *prev;
    UnkMBlock *next;
};
typedef struct { UnkMBlock *head; UnkMBlock *tail; } UnkMBlockList;
typedef struct { void *start; void *end; } UnkMemRegion;
typedef struct {
    UnkMBlockList freeList;
    UnkMBlockList usedList;
    u16 groupID;
    u16 feature;
} UnkExpHeap;
typedef struct {
    u32 signature;
    u8 link[8];
    u8 childList[0xC];
    void *heapStart;
    void *heapEnd;
    u32 attribute;
    UnkExpHeap exp;
} UnkHeapHead;

#define UNK_SET_BITS(data, st, bits, val) \
    do { \
        u16 v_ = (u16)(val); \
        (data) &= ~(((1 << (bits)) - 1) << (st)); \
        (data) |= (v_ & ((1 << (bits)) - 1)) << (st); \
    } while (0)

static inline void UnkSetBits(u16 *data, int st, int bits, u16 val) {
    *data &= ~(((1 << bits) - 1) << st);
    *data |= (val & ((1 << bits) - 1)) << st;
}

#endif
