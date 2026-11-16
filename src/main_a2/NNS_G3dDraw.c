#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct {
    u32 flag;
    void *resMdl;
    void *anm0;
    u32 unkC;
    void *anm1;
    u32 unk14;
    void *anm2;
    u8 unk1C[0x20];
    u32 blk0[2];
    u32 blk1[2];
    u32 blk2[2];
} UnkObj_02024cfc;

extern UnkRS *data_02084f9c;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02024df8(u32 *blk, void *anm);
void G3dDrawInternal_(UnkRS *rs, UnkObj_02024cfc *obj);

void NNS_G3dDraw(UnkObj_02024cfc *obj) {
    if ((obj->flag & 0x10) == 0x10) {
        {
            vu32 z = 0;
            MIi_CpuClear32(z, obj->blk0, 8);
        }
        {
            vu32 z = 0;
            MIi_CpuClear32(z, obj->blk1, 8);
        }
        {
            vu32 z = 0;
            MIi_CpuClear32(z, obj->blk2, 8);
        }
        if (obj->anm0 != NULL) func_02024df8(obj->blk0, obj->anm0);
        if (obj->anm1 != NULL) func_02024df8(obj->blk1, obj->anm1);
        if (obj->anm2 != NULL) func_02024df8(obj->blk2, obj->anm2);
        obj->flag &= ~0x10;
    }
    if (data_02084f9c != NULL) {
        G3dDrawInternal_(data_02084f9c, obj);
    } else {
        UnkRS rs;
        data_02084f9c = &rs;
        G3dDrawInternal_(&rs, obj);
        data_02084f9c = NULL;
    }
}
