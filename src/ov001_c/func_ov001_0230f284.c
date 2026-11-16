#include <nitro/types.h>

typedef struct UnkStruct_0230f284 {
    u8 pad0[0x60];
    u16 flags;
    u8 pad62[0x68 - 0x62];
    s16 type;
    u8 pad6A[0x70 - 0x6a];
    s32 unk70;
    u8 pad74[0xb0 - 0x74];
    u8 *unkB0;
    u8 padB4[0xcc - 0xb4];
    void *unkCC;
    s32 unkD0;
} UnkStruct_0230f284;

extern u8 *gSpecialStageGimmickModels;

void func_ov001_0230e3c8(UnkStruct_0230f284 *p, void *q, u16 ang);
void func_ov001_0230ea6c(void);

void func_ov001_0230f284(UnkStruct_0230f284 *p, void *q) {
    switch (p->type) {
    case 0:
    case 1:
        p->unkB0 = gSpecialStageGimmickModels + 0xf8;
        p->flags |= 8;
        p->unkCC = 0;
        p->unk70 = 0x14;
        break;
    case 2:
        p->unkB0 = gSpecialStageGimmickModels + 0xf8;
        p->flags |= 8;
        break;
    case 3:
        p->unkB0 = gSpecialStageGimmickModels + 0x3d4;
        p->flags |= 8;
        break;
    case 4:
    case 5:
    case 6:
        p->flags |= 8;
        break;
    case 0xf:
        func_ov001_0230e3c8(p, q, 0xd556);
        break;
    case 0x10:
        func_ov001_0230e3c8(p, q, 0xe000);
        break;
    case 0x11:
        func_ov001_0230e3c8(p, q, 0);
        break;
    case 0x12:
        func_ov001_0230e3c8(p, q, 0x2000);
        break;
    case 0x13:
        func_ov001_0230e3c8(p, q, 0x2aaa);
        break;
    case 0x17:
        p->flags |= 8;
        p->unkCC = func_ov001_0230ea6c;
        p->unkD0 = 0;
        break;
    }
}
