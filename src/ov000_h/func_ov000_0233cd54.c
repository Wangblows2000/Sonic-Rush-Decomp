#include <nitro/types.h>

typedef struct {
    u8 pad0[0xAC];
    u8 unkAC[0x200];
    u8 unk2AC[0x200];
    u8 unk4AC[0x200];
    u8 unk6AC[0x200];
    u8 *unk8AC;
    u8 *unk8B0;
} UnkStruct_0233cd54;

void func_02035704(void *src, s32 a, s32 b, u32 dst);

void func_ov000_0233cd54(UnkStruct_0233cd54 *w, s32 i) {
    u8 *p;

    if (i == 0) {
        func_02035704(w->unk6AC, 0xF0, 0, 0x5000000);
        p = w->unk2AC;
        func_02035704(p, 0xF0, 1, 0x4000);
        func_02035704(p, 0xF0, 1, 0x6000);
        func_02035704(w->unk8B0, 0, 0, 0x5000200);
        func_02035704(w->unk8B0 + 0x40, 0xE0, 0, 0x5000240);
    } else {
        func_02035704(w->unk6AC, 0xF0, 0, 0x5000400);
        p = w->unk2AC;
        func_02035704(p, 0xF0, 3, 0x4000);
        func_02035704(p, 0xF0, 3, 0x6000);
        func_02035704(w->unk8B0, 0, 0, 0x5000600);
        func_02035704(w->unk8B0 + 0x40, 0xE0, 0, 0x5000640);
    }
}
