#include <nitro/types.h>

typedef struct UnkStruct_022daaf4_r {
    u8 pad[0x54];
    u32 unk54;
} UnkStruct_022daaf4_r;

typedef struct UnkStruct_022daaf4_b {
    u8 pad[0x214];
    void *unk214;
    UnkStruct_022daaf4_r *unk218;
} UnkStruct_022daaf4_b;

typedef struct UnkStruct_022daaf4_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x6c - 0x32];
    u16 unk6C;
    u8 pad2[0x80 - 0x6e];
    u16 unk80;
    u8 pad3[0x92 - 0x82];
    u16 unk92;
} UnkStruct_022daaf4_c;

extern UnkStruct_022daaf4_b *data_ov000_02369b14;
extern UnkStruct_022daaf4_c data_ov000_0236a48c;

void func_ov000_022da6b4(void);
void func_ov000_022d9c70(UnkStruct_022daaf4_b *b);
void func_ov000_022d6644(void *p);

void func_ov000_022daaf4(void) {
    UnkStruct_022daaf4_r *r = data_ov000_02369b14->unk218;
    UnkStruct_022daaf4_c *c;
    if (r != NULL) {
        r->unk54 |= 8;
        data_ov000_02369b14->unk218 = NULL;
    }
    c = (UnkStruct_022daaf4_c *)(u32)&data_ov000_0236a48c;
    func_ov000_022da6b4();
    c->unk30 = 0;
    c->unk6C = 0;
    c->unk80 = 0;
    c->unk92 = 0;
    func_ov000_022d9c70(data_ov000_02369b14);
    func_ov000_022d6644(data_ov000_02369b14->unk214);
}
