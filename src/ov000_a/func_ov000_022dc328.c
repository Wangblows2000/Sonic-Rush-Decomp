#include <nitro/types.h>

typedef struct UnkStruct_022dc328_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u8 pad2[0x2c - 0x20];
} UnkStruct_022dc328_h;

typedef struct UnkStruct_022dc328 {
    u8 pad[0x134];
    UnkStruct_022dc328_h unk134;
    UnkStruct_022dc328_h unk160;
    u8 pad1[0x350 - 0x18c];
    UnkStruct_022dc328_h unk350;
    UnkStruct_022dc328_h unk37C;
} UnkStruct_022dc328;

typedef struct UnkTask_022dc328 {
    u8 pad[0x10];
    UnkStruct_022dc328 *unk10;
} UnkTask_022dc328;

extern UnkTask_022dc328 *gTaskCurrent;

void ObjRect_SetBox2D(UnkStruct_022dc328_h *h, s32 a, s32 b, s32 c, s32 d);
void StageObj_HandleCollider(UnkStruct_022dc328 *s, UnkStruct_022dc328_h *h);
void func_ov000_022db894(UnkStruct_022dc328_h *h, UnkStruct_022dc328_h *out);
void func_ov000_022dc3f0(void);

void func_ov000_022dc328(void) {
    UnkStruct_022dc328 *s = gTaskCurrent->unk10;
    UnkStruct_022dc328_h *h;
    UnkStruct_022dc328_h *h2;
    h = &s->unk134;
    ObjRect_SetBox2D(h, -0x18, -0x1b, 0x18, 0x1b);
    h->unk18 = s;
    h->unk1C = func_ov000_022dc3f0;
    h->unkA |= 4;
    StageObj_HandleCollider(s, h);
    h2 = &s->unk160;
    ObjRect_SetBox2D(h2, -0x20, -0x19, 0x20, 0x19);
    h2->unk18 = s;
    h2->unk1C = NULL;
    h2->unkA |= 4;
    StageObj_HandleCollider(s, h2);
    func_ov000_022db894(&s->unk134, &s->unk350);
    func_ov000_022db894(&s->unk160, &s->unk37C);
}
