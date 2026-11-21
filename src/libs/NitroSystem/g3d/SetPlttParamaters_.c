#include <nitro/types.h>

typedef struct { u16 ofs; u16 flag; } UnkPlttData_02028258;
typedef struct { u8 unk0[0x2c]; u32 vramKey; u8 unk30[4]; u16 ofsDict; } UnkTex_02028258;
typedef struct { u8 unk0[0x14]; u32 prmPlttBase; } UnkMatResult_02028258;

UnkPlttData_02028258 *NNS_G3dGetResDataByName(void *dict, const void *name);

void SetPlttParamaters_(UnkTex_02028258 *tex, const void *name, UnkMatResult_02028258 *res)
{
    UnkPlttData_02028258 *d;
    u32 ofs;
    u16 base;
    d = NNS_G3dGetResDataByName((u8 *)tex + tex->ofsDict, name);
    base = tex->vramKey;
    ofs = d->ofs;
    if ((d->flag & 1) == 0) {
        ofs = (u16)(ofs >> 1);
        base = (u16)(base >> 1);
    }
    res->prmPlttBase = ofs + base;
}
