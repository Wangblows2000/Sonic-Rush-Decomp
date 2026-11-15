#include <nitro/types.h>

typedef struct { u8 unk0[0xC]; u8 list[0xC]; u8 unk18[0xC]; } UnkStruct_0201c080;
extern UnkStruct_0201c080 data_020838a4[];
void *func_0201d9e4(void *s, u32 size, void *a, u32 b, u32 c);
void *func_0201db30(void *p, u32 size);
void func_0202076c(void *list, void *obj);
void func_0201b63c(void);

typedef struct { u8 link[8]; void *heap; u32 unkC; u32 unk10; u8 mem[1]; } UnkBlock_0201c080;

BOOL func_0201c080(u32 idx, void *s, u32 size) {
    UnkBlock_0201c080 *b = func_0201d9e4(s, size + 0x14, func_0201b63c, 0, 0);
    void *heap;
    if (b == NULL) return FALSE;
    b->unkC = 0;
    b->unk10 = idx;
    b->heap = NULL;
    heap = func_0201db30(b->mem, size);
    if (heap == NULL) return FALSE;
    b->heap = heap;
    func_0202076c(data_020838a4[idx].list, b);
    return TRUE;
}
