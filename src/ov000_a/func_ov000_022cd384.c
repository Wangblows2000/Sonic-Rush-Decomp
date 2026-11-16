#include <nitro/types.h>

typedef struct UnkStruct_022cd384_c {
    u8 pad[0x28];
    s32 unk28;
    u8 pad1[0x80 - 0x2c];
    u16 unk80;
    u8 pad2[0x8c - 0x82];
    u16 unk8C;
} UnkStruct_022cd384_c;

typedef struct UnkStruct_022cd384_2 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_022cd384_2;

typedef struct UnkStruct_022cd384_t {
    u16 a;
    u16 b;
    u16 c;
} UnkStruct_022cd384_t;

extern UnkStruct_022cd384_c data_ov000_0236a48c;
extern UnkStruct_022cd384_2 data_ov000_0236aad8;
extern UnkStruct_022cd384_t data_ov000_02365244[];
extern void *data_ov000_02369ae8[];
extern void *gTaskCurrent;

u8 *func_02025ecc(void *p);
void func_02025c44(u8 *p, u16 a);
void func_02025bfc(u8 *p, u16 a);
void func_02025bb4(u8 *p, u16 a);
void Task_Destroy(void *p);

void func_ov000_022cd384(void) {
    UnkStruct_022cd384_c *c = &data_ov000_0236a48c;
    s32 i;
    if ((data_ov000_0236aad8.unk62 << 12) < -c->unk28)
        return;
    for (i = 0; i < 11; i++) {
        u16 ta, tb, tc;
        tc = data_ov000_02365244[i].c;
        tb = data_ov000_02365244[i].b;
        ta = data_ov000_02365244[i].a;
        u8 *r = func_02025ecc(data_ov000_02369ae8[i]);
        u8 *q = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
        func_02025c44(q, ta);
        func_02025bfc(q, tb);
        func_02025bb4(q, tc);
    }
    c->unk80 = 0x200;
    c->unk8C = 0x80;
    Task_Destroy(gTaskCurrent);
}
