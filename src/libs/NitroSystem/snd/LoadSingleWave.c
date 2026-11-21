#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 fileSize; u8 unkC[0x2C]; u32 count; u32 table[1]; } UnkStruct_0201dc9c;

BOOL SND_GetWaveDataAddress(UnkStruct_0201dc9c *wa, u32 idx);
u32 SND_GetWaveDataCount(UnkStruct_0201dc9c *wa);
void *NNS_SndHeapAlloc(void *heap, u32 size, void *cb, void *arg, u32 c);
void SingleWaveDisposeCallback(void);
s32 NNS_SndArcReadFile(u32 idx, void *buf, u32 size, u32 pos);
void DC_StoreRange(void *p, u32 size);
void SND_SetWaveDataAddress(UnkStruct_0201dc9c *wa, u32 idx, void *mem);

BOOL LoadSingleWave(UnkStruct_0201dc9c *wa, u32 idx, u32 fileId, void *heap) {
    u32 size;
    u32 start;
    u32 end;
    u32 i;
    void *mem;
    if (SND_GetWaveDataAddress(wa, idx)) return TRUE;
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
    mem = NNS_SndHeapAlloc(heap, size + 0x20, SingleWaveDisposeCallback, wa, idx);
    if (mem == NULL) return FALSE;
    if (size != NNS_SndArcReadFile(fileId, mem, size, start)) return FALSE;
    DC_StoreRange(mem, size);
    SND_SetWaveDataAddress(wa, idx, mem);
    return TRUE;
}
