#include <nitro/types.h>

void *func_02021030(void *heap, u32 size, s32 align);
void *NNS_FndGetPrevListObject(void *list, void *obj);
void func_0202076c(void *list, void *obj);

typedef struct { u8 link[8]; u32 size; u32 unkC; u32 unk10; u32 unk14; u8 unk18[8]; } UnkBlock_0201d9e4;
typedef struct { void *heap; u8 list[0xC]; } UnkStruct_0201d9e4;

void *func_0201d9e4(UnkStruct_0201d9e4 *s, u32 size, u32 a, u32 b, u32 c) {
    UnkBlock_0201d9e4 *blk = func_02021030(s->heap, ((size + 0x1F) & ~0x1F) + 0x20, 0x20);
    void *tail;
    if (blk == NULL) return NULL;
    tail = NNS_FndGetPrevListObject(s->list, NULL);
    blk->size = size;
    blk->unkC = a;
    blk->unk10 = b;
    blk->unk14 = c;
    func_0202076c(tail, blk);
    return blk + 1;
}
