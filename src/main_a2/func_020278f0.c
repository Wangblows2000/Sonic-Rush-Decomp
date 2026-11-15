#include <nitro/types.h>

typedef struct { u8 unk0[6]; u16 num; u8 unk8[0xC]; u16 ofs[1]; } UnkResAnm_020278f0;
typedef struct { u8 unk0[0x17]; u8 numMat; } UnkMdl_020278f0;
typedef struct {
    u8 unk0[8];
    UnkResAnm_020278f0 *resAnm;
    void *funcAnm;
    u8 unk10[9];
    u8 numMapData;
    u16 mapData[1];
} UnkAnmObj_020278f0;

extern void *data_02077acc;

void MIi_CpuClear16(u16 data, void *dest, u32 size);

void func_020278f0(UnkAnmObj_020278f0 *obj, UnkResAnm_020278f0 *anm, UnkMdl_020278f0 *mdl) {
    u32 i;
    vu16 z;
    obj->resAnm = anm;
    obj->funcAnm = data_02077acc;
    obj->numMapData = mdl->numMat;
    z = 0;
    MIi_CpuClear16(z, obj->mapData, obj->numMapData * 2);
    {
        const u16 *ofs = anm->ofs;
        for (i = 0; i < anm->num; i++) {
            obj->mapData[i] = (*(u32 *)((u8 *)anm + ofs[i]) >> 24) | 0x100;
        }
    }
}
