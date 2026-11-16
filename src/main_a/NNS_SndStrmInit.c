#include <nitro/types.h>

void NNS_FndInitList(void *list, u16 offset);
void func_0201c1d0(void *p);
void func_0201c158(void *p);
extern u32 data_02083d24;
extern u8 data_02083d28[];

typedef struct { u8 unk0[8]; void *unk8; void *unkC; u32 unk10; void *unk14; void *unk18; u8 unk1C[8]; u32 unk24; u8 unk28[0x1C]; u32 unk44; u32 unk48; } UnkStruct_0201c748;

void NNS_SndStrmInit(UnkStruct_0201c748 *s) {
    if (data_02083d24 == 0) {
        NNS_FndInitList(data_02083d28, 0);
        data_02083d24 = 1;
    }
    s->unk8 = func_0201c1d0;
    s->unkC = s;
    s->unk14 = func_0201c158;
    s->unk18 = s;
    s->unk44 = 0;
    s->unk48 = 0;
    s->unk24 &= ~1;
    s->unk24 &= ~2;
}
