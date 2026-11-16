#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; u16 unk14; } UnkStruct_02035704;
UnkStruct_02035704 *PaletteTransfer_AllocJob(void);
UnkStruct_02035704 *func_02006abc(u32 a);

void PaletteTransfer_QueueCopy(u32 a, u32 b, u32 c, u32 d) {
    UnkStruct_02035704 *s = PaletteTransfer_AllocJob();
    if (func_02006abc(0) == s) return;
    s->unk8 = c;
    s->unkC = d;
    s->unk4 = 0;
    s->unk10 = a;
    s->unk14 = b;
}
