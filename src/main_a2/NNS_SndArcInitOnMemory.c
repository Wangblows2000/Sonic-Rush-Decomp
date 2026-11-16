#include <nitro/types.h>

typedef struct { u32 offset; u32 size; void *mem; u32 reserved; } UnkFatEntry_0201d580;
typedef struct { u8 header[8]; u32 count; UnkFatEntry_0201d580 entries[1]; } UnkFat_0201d580;

typedef struct {
    u8 unk0[0x10];
    u32 symbOff;
    u32 symbSize;
    u32 infoOff;
    u32 infoSize;
    u32 fatOff;
    u32 fatSize;
    u8 unk28[8];
    u32 unk30;
    u8 file[0x4C];
    UnkFat_0201d580 *fat;
    void *symb;
    void *info;
} UnkStruct_0201d580;

extern UnkStruct_0201d580 *data_02083f04;

void MIi_CpuCopy32(const void *src, void *dst, u32 size);

void NNS_SndArcInitOnMemory(UnkStruct_0201d580 *arc, u8 *base) {
    u32 i;
    MIi_CpuCopy32(base, arc, 0x30);
    arc->info = arc->infoOff == 0 ? NULL : base + arc->infoOff;
    arc->fat = arc->fatOff == 0 ? NULL : (UnkFat_0201d580 *)(base + arc->fatOff);
    arc->symb = arc->symbOff == 0 ? NULL : base + arc->symbOff;
    for (i = 0; i < arc->fat->count; i++) {
        UnkFatEntry_0201d580 *e = &arc->fat->entries[i];
        e->mem = e->offset == 0 ? NULL : base + e->offset;
    }
    arc->unk30 = 0;
    data_02083f04 = arc;
}
