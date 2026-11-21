#include <nitro/types.h>

typedef void (*UnkCb_0201da54)(void *mem, u32 size, u32 a, u32 b);
typedef struct { u8 link[8]; u32 size; UnkCb_0201da54 cb; u32 a; u32 b; u8 unk18[8]; } UnkBlock_0201da54;
typedef struct { void *heap; u8 list[0xC]; } UnkStruct_0201da54;

void *NNS_FndGetPrevListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);
void NNS_FndFreeToFrmHeap(void *heap, u32 a);
void EraseSync(void);
void NewSection(UnkStruct_0201da54 *h);

void NNS_SndHeapClear(UnkStruct_0201da54 *h) {
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
        NNS_FndRemoveListObject(h->list, group);
    }
    NNS_FndFreeToFrmHeap(h->heap, 3);
    if (called) EraseSync();
    NewSection(h);
}
