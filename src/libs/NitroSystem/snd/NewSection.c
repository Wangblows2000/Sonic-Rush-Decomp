#include <nitro/types.h>

void *NNS_FndAllocFromFrmHeapEx(void *heap, u32 size, s32 align);
void InitHeapSection(void *p);
void NNS_FndAppendListObject(void *list, void *obj);

typedef struct { void *heap; u8 list[0xC]; } UnkStruct_0201d914;

BOOL NewSection(UnkStruct_0201d914 *s) {
    void *p = NNS_FndAllocFromFrmHeapEx(s->heap, 0x14, 4);
    if (p == NULL) return FALSE;
    InitHeapSection(p);
    NNS_FndAppendListObject(s->list, p);
    return TRUE;
}
