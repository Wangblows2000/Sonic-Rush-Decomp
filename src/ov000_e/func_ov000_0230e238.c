#include <nitro/types.h>

typedef struct UnkStruct_0230e238 {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x68 - 0x10];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    struct UnkStruct_0230e238 *unk88;
} UnkStruct_0230e238;

void func_02069818(void *w, s32 a);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_ov000_0230e7ec(void);

void func_ov000_0230e238(UnkStruct_0230e238 *s) {
    UnkStruct_0230e238 *p = s->unk88;
    if (p == NULL) {
        return;
    }
    if (p->unk68 != func_ov000_0230e7ec) {
        return;
    }
    if (s->unkC > 0xb5 && p->unkC <= 0xb5) {
        func_02069818(s, 2);
        func_0204b7ec(0, 0, 0x6b);
    }
    if (s->unkC > 0x79 && s->unk88->unkC <= 0x79) {
        func_02069818(s, 3);
        func_0204b7ec(0, 0, 0x6b);
    }
    if (s->unkC > 0x3d && s->unk88->unkC <= 0x3d) {
        func_02069818(s, 4);
        func_0204b7ec(0, 0, 0x6b);
    }
    if (s->unkC > 1 && s->unk88->unkC <= 1) {
        func_02069818(s, 5);
        func_0204b7ec(0, 0, 0xeb);
    }
    s->unkC = s->unk88->unkC;
}
