#include <nitro/types.h>

typedef struct UnkStruct_022dc9fc_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u8 pad2[0x2c - 0x20];
} UnkStruct_022dc9fc_h;

typedef struct UnkStruct_022dc9fc {
    u8 pad[0x160];
    UnkStruct_022dc9fc_h unk160;
    UnkStruct_022dc9fc_h unk18C;
} UnkStruct_022dc9fc;

typedef struct UnkTask_022dc9fc {
    u8 pad[0x10];
    UnkStruct_022dc9fc *unk10;
} UnkTask_022dc9fc;

extern UnkTask_022dc9fc *gTaskCurrent;

void ObjRect_SetBox2D(UnkStruct_022dc9fc_h *h, s32 a, s32 b, s32 c, s32 d);
void StageObj_HandleCollider(UnkStruct_022dc9fc *s, UnkStruct_022dc9fc_h *h);
void func_ov000_022db894(UnkStruct_022dc9fc_h *h, UnkStruct_022dc9fc_h *out);

void func_ov000_022dc9fc(void) {
    UnkStruct_022dc9fc *s = gTaskCurrent->unk10;
    UnkStruct_022dc9fc_h *h = &s->unk160;
    ObjRect_SetBox2D(h, -0xb, -0x16, 0xb, 0x16);
    h->unk18 = s;
    h->unk1C = NULL;
    h->unkA |= 4;
    StageObj_HandleCollider(s, h);
    func_ov000_022db894(&s->unk160, &s->unk18C);
}
