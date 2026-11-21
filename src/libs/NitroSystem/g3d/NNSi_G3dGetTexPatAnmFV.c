#include <nitro/types.h>

typedef struct { u16 count; u16 unk2; s16 scale; u16 offset; } UnkInfo_020261d4;
UnkInfo_020261d4 *NNSi_G3dGetTexPatAnmDataByIdx(void *p, u32 idx);

u16 *NNSi_G3dGetTexPatAnmFV(u8 *p, u32 idx, u32 key) {
    UnkInfo_020261d4 *info = NNSi_G3dGetTexPatAnmDataByIdx(p, idx);
    u16 *tbl = (u16 *)(p + info->offset);
    u32 i = (info->scale * key) >> 12;
    while (i != 0 && tbl[i * 2] >= key) {
        i--;
    }
    while (i + 1 < info->count && tbl[(i + 1) * 2] <= key) {
        i++;
    }
    return &tbl[i * 2];
}
