#include <nitro/types.h>

void *func_02021030(void *heap, u32 size, s32 align);
void func_0201d9a4(void *p);
void func_0202076c(void *list, void *obj);

typedef struct { void *heap; u8 list[0xC]; } UnkStruct_0201d914;

BOOL func_0201d914(UnkStruct_0201d914 *s) {
    void *p = func_02021030(s->heap, 0x14, 4);
    if (p == NULL) return FALSE;
    func_0201d9a4(p);
    func_0202076c(s->list, p);
    return TRUE;
}
