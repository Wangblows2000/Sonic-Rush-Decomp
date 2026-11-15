#include <nitro/types.h>

typedef struct { u8 unk0[0x38]; u32 count; u32 table[1]; } UnkStruct_0201df20;
extern UnkStruct_0201df20 data_02083f08;

UnkStruct_0201df20 *func_0201d0f8(u32 fileId);
s32 func_0201d140(u32 idx, void *buf, u32 size, u32 pos);
u32 func_0201d558(void);
void *func_0201d9e4(void *heap, u32 size, void *cb, u32 arg, u32 c);
void func_0201dde4(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void MI_CpuFill8(void *dst, u8 val, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_0201d0dc(u32 fileId, void *p);

UnkStruct_0201df20 *func_0201df20(u32 fileId, void *heap, BOOL cache) {
    UnkStruct_0201df20 *wa;
    u32 total;
    u32 tsize;
    u32 n;
    wa = func_0201d0f8(fileId);
    if (wa == NULL) {
        if (func_0201d140(fileId, &data_02083f08, 0x3C, 0) != 0x3C) return NULL;
        tsize = data_02083f08.count * 4;
        total = tsize * 2 + 0x3C;
        if (heap == NULL) return NULL;
        wa = func_0201d9e4(heap, total + 0x20, func_0201dde4, cache ? func_0201d558() : 0, fileId);
        if (wa == NULL) return NULL;
        n = func_0201d140(fileId, wa, tsize + 0x3C, 0);
        if (n != tsize + 0x3C) return NULL;
        MI_CpuCopy8(wa->table, wa->table + wa->count, tsize);
        MI_CpuFill8(wa->table, 0, tsize);
        DC_StoreRange(wa, total);
        if (cache) func_0201d0dc(fileId, wa);
    }
    return wa;
}
