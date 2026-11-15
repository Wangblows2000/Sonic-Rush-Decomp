#include <nitro/types.h>

typedef void (*UnkCb_0201da54)(void *mem, u32 size, u32 a, u32 b);
typedef struct { u8 link[8]; u32 size; UnkCb_0201da54 cb; u32 a; u32 b; u8 unk18[8]; } UnkBlock_0201da54;
typedef struct { void *heap; u8 list[0xC]; } UnkStruct_0201da54;

void *NNS_FndGetPrevListObject(void *list, void *obj);
void func_0202061c(void *list, void *obj);
void func_02020ff0(void *heap, u32 a);
void func_0201d8f0(void);
void func_0201d914(UnkStruct_0201da54 *h);

void func_0201da54(UnkStruct_0201da54 *h) {
    void *group;
    UnkBlock_0201da54 *blk;
    BOOL called;
    called = FALSE;
    while ((group = NNS_FndGetPrevListObject(h->list, NULL)) != NULL) {
        blk = NULL;
        while ((blk = NNS_FndGetPrevListObject(group, blk)) != NULL) {
            if (blk->cb != NULL) {
                blk->cb(blk + 1, blk->size, blk->a, blk->b);
                called = TRUE;
            }
        }
        func_0202061c(h->list, group);
    }
    func_02020ff0(h->heap, 3);
    if (called) func_0201d8f0();
    func_0201d914(h);
}
