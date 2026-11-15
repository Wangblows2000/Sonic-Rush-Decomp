#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_0202839c;
typedef struct { u16 sizeUnit; u16 ofsName; } UnkDictEntry_0202839c;
typedef struct { u8 unk0[0xC]; UnkDict_0202839c dict; } UnkResAnm_0202839c;
typedef struct { u8 unk0[8]; u32 ofsMat; u8 unkC[0xC]; u8 numMat; } UnkMdl_0202839c;
typedef struct { u8 unk0[4]; UnkDict_0202839c dict; } UnkResMat_0202839c;
typedef struct {
    u8 unk0[8];
    void *resAnm;
    void *funcAnm;
    u8 unk10[9];
    u8 numMapData;
    u16 mapData[1];
} UnkAnmObj_0202839c;

extern void *data_02077ad4;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
s32 func_02025edc(const UnkDict_0202839c *dict, const void *name);

void func_0202839c(UnkAnmObj_0202839c *obj, UnkResAnm_0202839c *anm, UnkMdl_0202839c *mdl) {
    UnkResMat_0202839c *mat = (UnkResMat_0202839c *)((u8 *)mdl + mdl->ofsMat);
    u32 i;
    vu16 z;
    obj->funcAnm = data_02077ad4;
    obj->numMapData = mdl->numMat;
    obj->resAnm = anm;
    z = 0;
    MIi_CpuClear16(z, obj->mapData, obj->numMapData * 2);
    for (i = 0; i < anm->dict.numEntry; i++) {
        UnkDictEntry_0202839c *e = (UnkDictEntry_0202839c *)((u8 *)&anm->dict + anm->dict.ofsEntry);
        s32 idx = func_02025edc(&mat->dict, (u8 *)e + e->ofsName + i * 16);
        if (idx >= 0) obj->mapData[idx] = i | 0x100;
    }
}
