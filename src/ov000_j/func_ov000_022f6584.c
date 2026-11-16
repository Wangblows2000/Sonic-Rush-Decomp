#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x68 - 0x10];
    void *unk68;
    u8 pad6C[0x218 - 0x6c];
    s32 unk218;
    u8 pad21C[0x35c - 0x21c];
    s32 unk35C;
} UnkStruct_022f6584;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x58 - 0x14];
    u32 unk58;
} UnkStruct_022f6584_plr;

extern UnkStruct_022f6584_plr *data_ov000_0236ac60;

void func_ov000_022f6b8c(UnkStruct_022f6584 *p);
void func_ov000_022f6978(UnkStruct_022f6584 *p);
void func_ov000_022f653c(UnkStruct_022f6584 *p);

void func_ov000_022f6584(UnkStruct_022f6584 *p) {
    UnkStruct_022f6584_plr *plr;

    func_ov000_022f6b8c(p);
    func_ov000_022f6978(p);
    if (p->unkC++ <= 0x3c)
        return;
    plr = data_ov000_0236ac60;
    if (!(plr->unk58 & 1))
        return;
    if (p->unk218 >= plr->unk10)
        return;
    if (plr->unk10 < p->unk35C)
        p->unk68 = func_ov000_022f653c;
}
