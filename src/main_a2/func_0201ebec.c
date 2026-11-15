#include <nitro/types.h>

typedef struct { u8 seqMax; u8 pad; u16 allocChBitFlag; u32 heapSize; } UnkPlayerInfo_0201ebec;

u32 func_0201d558(void);
UnkPlayerInfo_0201ebec *func_0201d2dc(s32 idx);
void func_0201c138(s32 idx, u8 v);
void func_0201c120(s32 idx, u16 v);
BOOL func_0201c080(s32 idx, void *heap, u32 size);

BOOL func_0201ebec(void *heap) {
    s32 i;
    func_0201d558();
    for (i = 0; i < 0x20; i++) {
        UnkPlayerInfo_0201ebec *info = func_0201d2dc(i);
        if (info == NULL) continue;
        func_0201c138(i, info->seqMax);
        func_0201c120(i, info->allocChBitFlag);
        if (info->heapSize != 0 && heap != NULL) {
            s32 j;
            for (j = 0; j < info->seqMax; j++) {
                if (!func_0201c080(i, heap, info->heapSize)) return FALSE;
            }
        }
    }
    return TRUE;
}
