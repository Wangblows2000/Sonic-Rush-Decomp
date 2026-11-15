#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct {
    u8 unk0[4];
    u32 ofsSbc;
    u32 ofsMat;
    u32 ofsShp;
    u8 unk10[5];
    u8 scalingRule;
    u8 texMtxMode;
    u8 unk17[5];
    u32 unk1C;
    u32 unk20;
} UnkResMdl_02024e70;

typedef struct {
    u32 flag;
    UnkResMdl_02024e70 *resMdl;
    u8 unk8[0x18];
    UnkRSCb cb;
    u8 cbCmd;
    u8 cbTiming;
    u8 unk26[2];
    UnkRSCb cbInitRS;
    u8 unk2C[4];
    u8 *sbc;
} UnkObj_02024e70;

extern void *data_02077b44[];
extern void *data_02077b38[];
extern void *data_02077b50[];

void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void func_02025004(UnkRS *rs);

void func_02024e70(UnkRS *rs, UnkObj_02024e70 *obj) {
    vu32 z = 0;
    MIi_CpuClearFast(z, rs, sizeof(UnkRS));
    rs->unkC4 = 1;
    rs->flag = 1;
    if (obj->sbc != NULL) {
        rs->c = obj->sbc;
    } else {
        rs->c = (u8 *)obj->resMdl + obj->resMdl->ofsSbc;
    }
    rs->obj = obj;
    rs->unkD4 = (u8 *)obj->resMdl + 0x40;
    rs->resMat = (u8 *)obj->resMdl + obj->resMdl->ofsMat;
    rs->resShp = (u8 *)obj->resMdl + obj->resMdl->ofsShp;
    rs->unkE8 = data_02077b44[obj->resMdl->scalingRule];
    rs->unkEC = data_02077b38[obj->resMdl->scalingRule];
    rs->unkF0 = data_02077b50[obj->resMdl->texMtxMode];
    rs->unkE0 = obj->resMdl->unk1C;
    rs->unkE4 = obj->resMdl->unk20;
    if (obj->cb != NULL && obj->cbCmd < 0x20) {
        rs->cb[obj->cbCmd] = obj->cb;
        rs->timing[obj->cbCmd] = obj->cbTiming;
    }
    if (obj->flag & 1) rs->flag |= 0x80;
    if (obj->flag & 2) rs->flag |= 0x100;
    if (obj->flag & 4) rs->flag |= 0x200;
    if (obj->flag & 8) rs->flag |= 0x400;
    if (obj->cbInitRS != NULL) obj->cbInitRS(rs);
    func_02025004(rs);
    obj->flag &= ~1;
}
