#include <nitro/types.h>

typedef struct { u8 unk0[0xC]; u8 list[0xC]; u8 unk18[0xC]; } UnkStruct_0201c080;
extern UnkStruct_0201c080 data_020838a4[];
void *NNS_SndHeapAlloc(void *s, u32 size, void *a, u32 b, u32 c);
void *NNS_SndHeapCreate(void *p, u32 size);
void NNS_FndAppendListObject(void *list, void *obj);
void PlayerHeapDisposeCallback(void);

typedef struct { u8 link[8]; void *heap; u32 unkC; u32 unk10; u8 mem[1]; } UnkBlock_0201c080;

BOOL NNS_SndPlayerCreateHeap(u32 idx, void *s, u32 size) {
    UnkBlock_0201c080 *b = NNS_SndHeapAlloc(s, size + 0x14, PlayerHeapDisposeCallback, 0, 0);
    void *heap;
    if (b == NULL) return FALSE;
    b->unkC = 0;
    b->unk10 = idx;
    b->heap = NULL;
    heap = NNS_SndHeapCreate(b->mem, size);
    if (heap == NULL) return FALSE;
    b->heap = heap;
    NNS_FndAppendListObject(data_020838a4[idx].list, b);
    return TRUE;
}
