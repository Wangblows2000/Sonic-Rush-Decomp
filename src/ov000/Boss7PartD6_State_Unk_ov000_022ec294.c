#include <nitro/types.h>

typedef struct UnkStruct_022ec294 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x4c];
    u16 unk5C;
    u8 pad2[0xa];
    void (*func)(void);
} UnkStruct_022ec294;

void func_ov000_022ec1d0(void);

void Boss7PartD6_State_Unk_ov000_022ec294(UnkStruct_022ec294 *s) {
    if (s->unkC > 0)
        s->unkC--;
    if (!(s->unk5C & 8))
        return;
    if (s->unkC == 0)
        s->func = func_ov000_022ec1d0;
}
