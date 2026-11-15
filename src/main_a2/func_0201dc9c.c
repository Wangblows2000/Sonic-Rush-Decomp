#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 fileSize; u8 unkC[0x2C]; u32 count; u32 table[1]; } UnkStruct_0201dc9c;

BOOL func_02009b60(UnkStruct_0201dc9c *wa, u32 idx);
u32 SND_GetWaveDataCount(UnkStruct_0201dc9c *wa);
void *func_0201d9e4(void *heap, u32 size, void *cb, void *arg, u32 c);
void func_0201dd90(void);
s32 func_0201d140(u32 idx, void *buf, u32 size, u32 pos);
void DC_StoreRange(void *p, u32 size);
void func_02009ba8(UnkStruct_0201dc9c *wa, u32 idx, void *mem);

BOOL func_0201dc9c(UnkStruct_0201dc9c *wa, u32 idx, u32 fileId, void *heap) {
    u32 size;
    u32 start;
    u32 end;
    u32 i;
    void *mem;
    if (func_02009b60(wa, idx)) return TRUE;
    end = SND_GetWaveDataCount(wa);
    i = wa->count + idx;
    start = wa->table[i];
    if (idx < end - 1) {
        end = wa->table[i + 1];
    } else {
        end = wa->fileSize;
    }
    size = end - start;
    if (heap == NULL) return FALSE;
    mem = func_0201d9e4(heap, size + 0x20, func_0201dd90, wa, idx);
    if (mem == NULL) return FALSE;
    if (size != func_0201d140(fileId, mem, size, start)) return FALSE;
    DC_StoreRange(mem, size);
    func_02009ba8(wa, idx, mem);
    return TRUE;
}
