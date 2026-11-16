#include <nitro/types.h>

typedef struct UnkStruct_022dd458_w UnkStruct_022dd458_w;
struct UnkStruct_022dd458_w {
    u8 pad[0x68];
    void (*func)(UnkStruct_022dd458_w *w);
    u8 pad1[0x224 - 0x6c];
    s32 unk224;
};

typedef struct UnkStruct_022dd458_c {
    u8 pad[0x78];
    s32 unk78;
    u8 pad1[0x84 - 0x7c];
    s32 unk84;
    u8 pad2[0x8e - 0x88];
    u16 unk8E;
} UnkStruct_022dd458_c;

typedef struct UnkStruct_022dd458_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022dd458_g;

extern UnkStruct_022dd458_w *gBoss4;
extern UnkStruct_022dd458_c data_ov000_0236a48c;
extern UnkStruct_022dd458_g gDisplayMain;

void Boss4_State_Unk_ov000_022dd410(UnkStruct_022dd458_w *w);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c5c80(s32 a);

void func_ov000_022dd458(void) {
    UnkStruct_022dd458_w *w = gBoss4;
    UnkStruct_022dd458_c *c;
    w->func = Boss4_State_Unk_ov000_022dd410;
    w->unk224 = 0;
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    func_ov000_022c5c80(6);
    c = (UnkStruct_022dd458_c *)(u32)&data_ov000_0236a48c;
    c->unk78 = 0x40000;
    c->unk84 = 0x10000;
    c->unk8E = 0x4000;
}
