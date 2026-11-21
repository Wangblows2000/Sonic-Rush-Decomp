#include <nitro/types.h>

typedef struct { u32 fileId : 24; u32 flags : 8; } UnkInfo_0201e3a0;
typedef struct { u32 fileId; u16 waveArc[4]; } UnkBankInfo_0201e3a0;

UnkBankInfo_0201e3a0 *NNS_SndArcGetBankInfo(s32 idx);
UnkInfo_0201e3a0 *NNS_SndArcGetWaveArcInfo(s32 idx);
void *LoadBank(u32 fileId, void *heap, BOOL cache);
void *NNS_SndArcGetFileAddress(u32 fileId);
s32 NNSi_SndArcLoadWaveArc(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
BOOL LoadSingleWaves(void *wa, void *bank, u32 waveArc, u32 fileId, void *heap);
void SND_AssignWaveArc(void *bank, s32 i, void *wa);

s32 NNSi_SndArcLoadBank(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkBankInfo_0201e3a0 *info;
    void *bank;
    s32 i;
    UnkInfo_0201e3a0 *waInfo;
    void *wa;
    s32 r;

    info = NNS_SndArcGetBankInfo(idx);
    if (info == NULL) return 4;
    if (flags & 2) {
        bank = LoadBank(info->fileId, heap, cache);
        if (bank == NULL) return 8;
    } else {
        bank = NNS_SndArcGetFileAddress(info->fileId);
    }
    for (i = 0; i < 4; i++) {
        if (info->waveArc[i] == 0xFFFF) continue;
        waInfo = NNS_SndArcGetWaveArcInfo(info->waveArc[i]);
        if (waInfo == NULL) return 5;
        r = NNSi_SndArcLoadWaveArc(info->waveArc[i], flags, heap, cache, &wa);
        if (r != 0) return r;
        if ((waInfo->flags & 1) && (flags & 4)) {
            if (!LoadSingleWaves(wa, bank, i, waInfo->fileId, heap)) return 9;
        }
        if (bank != NULL && wa != NULL) SND_AssignWaveArc(bank, i, wa);
    }
    if (out != NULL) *out = bank;
    return 0;
}
