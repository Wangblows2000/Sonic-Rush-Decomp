#ifndef OV000_D_MDL2_H
#define OV000_D_MDL2_H

#include <ov000_d/mdl.h>

/* ov000 0x0235d4xx two-model menu object */
typedef struct {
    u8 pad0[0x67C];
    UnkStruct_MdlRender rA;
    u8 pad714[0x788 - 0x67C - sizeof(UnkStruct_MdlRender)];
    u8 flagA;
    u8 pad789[0x7B4 - 0x789];
    UnkStruct_MdlRender rB;
    u8 pad84C[0x8C0 - 0x7B4 - sizeof(UnkStruct_MdlRender)];
    u8 flagB;
    u8 pad8C1[0x8EC - 0x8C1];
    u16 angA;
    u16 angB;
    void *mdl8F0;
    void *mdl8F4;
    void *anm8F8;
    void *anm8FC;
    void *anm900;
    void *anm904;
    void *unk908;
    void *unk90C;
} UnkStruct_Mdl2;

#endif
