#include <nitro/types.h>

typedef struct UnkStruct_022cd448_c {
    u8 pad[0x28];
    s32 unk28;
    u8 pad1[0x80 - 0x2c];
    u16 unk80;
    u8 pad2[0x8c - 0x82];
    u16 unk8C;
} UnkStruct_022cd448_c;

typedef struct UnkStruct_022cd448_2 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_022cd448_2;

typedef struct UnkStruct_022cd448_t {
    u16 a;
    u16 b;
    u16 c;
} UnkStruct_022cd448_t;

extern UnkStruct_022cd448_c data_ov000_0236a48c;
extern UnkStruct_022cd448_2 data_ov000_0236aad8;
extern UnkStruct_022cd448_t data_ov000_02362fd8[];
extern void *data_ov000_02369ae8[];
extern void *gTaskCurrent;

u8 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dMdlSetMdlDiffAll(u8 *p, u16 a);
void NNS_G3dMdlSetMdlAmbAll(u8 *p, u16 a);
void NNS_G3dMdlSetMdlEmiAll(u8 *p, u16 a);
void Task_Destroy(void *p);

void func_ov000_022cd448(void) {
    UnkStruct_022cd448_c *c = &data_ov000_0236a48c;
    s32 i;
    if ((data_ov000_0236aad8.unk62 << 12) > -c->unk28)
        return;
    for (i = 0; i < 11; i++) {
        u8 *r = NNS_G3dGetMdlSet(data_ov000_02369ae8[i]);
        u8 *q = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
        NNS_G3dMdlSetMdlDiffAll(q, data_ov000_02362fd8[i].a);
        NNS_G3dMdlSetMdlAmbAll(q, data_ov000_02362fd8[i].b);
        NNS_G3dMdlSetMdlEmiAll(q, data_ov000_02362fd8[i].c);
    }
    c->unk80 = 0x200;
    c->unk8C = 0x80;
    Task_Destroy(gTaskCurrent);
}
