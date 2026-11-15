#include <nitro/types.h>

typedef struct { u32 fileId : 24; u32 flags : 8; } UnkInfo_0201e3a0;
typedef struct { u32 fileId; u16 waveArc[4]; } UnkBankInfo_0201e3a0;

UnkBankInfo_0201e3a0 *func_0201d408(s32 idx);
UnkInfo_0201e3a0 *func_0201d3a4(s32 idx);
void *func_0201e0d0(u32 fileId, void *heap, BOOL cache);
void *func_0201d0f8(u32 fileId);
s32 func_0201e2f8(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
BOOL func_0201dbcc(void *wa, void *bank, u32 waveArc, u32 fileId, void *heap);
void func_02009ecc(void *bank, s32 i, void *wa);

s32 func_0201e3a0(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkBankInfo_0201e3a0 *info;
    void *bank;
    s32 i;
    UnkInfo_0201e3a0 *waInfo;
    void *wa;
    s32 r;

    info = func_0201d408(idx);
    if (info == NULL) return 4;
    if (flags & 2) {
        bank = func_0201e0d0(info->fileId, heap, cache);
        if (bank == NULL) return 8;
    } else {
        bank = func_0201d0f8(info->fileId);
    }
    for (i = 0; i < 4; i++) {
        if (info->waveArc[i] == 0xFFFF) continue;
        waInfo = func_0201d3a4(info->waveArc[i]);
        if (waInfo == NULL) return 5;
        r = func_0201e2f8(info->waveArc[i], flags, heap, cache, &wa);
        if (r != 0) return r;
        if ((waInfo->flags & 1) && (flags & 4)) {
            if (!func_0201dbcc(wa, bank, i, waInfo->fileId, heap)) return 9;
        }
        if (bank != NULL && wa != NULL) func_02009ecc(bank, i, wa);
    }
    if (out != NULL) *out = bank;
    return 0;
}
