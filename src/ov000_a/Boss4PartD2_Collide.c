#include <nitro/types.h>

typedef struct UnkStruct_022dbd5c_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u8 pad2[0x2c - 0x20];
} UnkStruct_022dbd5c_h;

typedef struct UnkStruct_022dbd5c {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x160 - 0x14];
    UnkStruct_022dbd5c_h unk160;
    UnkStruct_022dbd5c_h unk18C;
    u8 pad2[0x480 - 0x1b8];
    UnkStruct_022dbd5c_h unk480;
    UnkStruct_022dbd5c_h unk4AC;
    u8 pad3[0x4e0 - 0x4d8];
    s32 unk4E0;
    s32 unk4E4;
} UnkStruct_022dbd5c;

typedef struct UnkTask_022dbd5c {
    u8 pad[0x10];
    UnkStruct_022dbd5c *unk10;
} UnkTask_022dbd5c;

extern UnkTask_022dbd5c *gTaskCurrent;

void ObjRect_SetBox2D(UnkStruct_022dbd5c_h *h, s32 a, s32 b, s32 c, s32 d);
void StageObj_HandleCollider(UnkStruct_022dbd5c *s, UnkStruct_022dbd5c_h *h);
void Boss4_Unk_ov000_022db894(UnkStruct_022dbd5c_h *h, UnkStruct_022dbd5c_h *out);

void Boss4PartD2_Collide(void) {
    UnkStruct_022dbd5c *s = gTaskCurrent->unk10;
    UnkStruct_022dbd5c_h *h;
    s->unk10 = s->unk4E0;
    h = &s->unk160;
    ObjRect_SetBox2D(h, -0x50, -0x41, 0, 10);
    h->unk18 = s;
    h->unk1C = NULL;
    h->unkA |= 4;
    StageObj_HandleCollider(s, h);
    Boss4_Unk_ov000_022db894(&s->unk160, &s->unk480);
    s->unk10 = s->unk4E4;
    h = &s->unk18C;
    ObjRect_SetBox2D(h, 0, -0x41, 0x50, 10);
    h->unk18 = s;
    h->unk1C = NULL;
    h->unkA |= 4;
    StageObj_HandleCollider(s, h);
    Boss4_Unk_ov000_022db894(&s->unk18C, &s->unk4AC);
}
