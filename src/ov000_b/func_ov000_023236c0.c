#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x184 - 0x124];
    UnkStruct_ObjParam *unk184;
    u8 pad188[0x19C - 0x188];
} UnkStruct_023236c0;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367468[], data_ov000_0236a700[], data_ov000_0236a7e0[];
extern u8 data_ov000_02367480[], data_ov000_0236a7f0[];
extern u8 data_ov000_02367498[], data_ov000_0236a800[];
extern u8 data_ov000_023674b0[], data_ov000_0236a810[];
extern u8 data_ov000_023674c8[], data_ov000_0236a820[];
extern u8 data_ov000_023674e0[], data_ov000_0236a830[];
extern u8 data_ov000_023674f8[], data_ov000_0236a6f0[], data_ov000_02367518[], data_ov000_0236a790[];
extern u8 data_ov000_02367538[], data_ov000_0236a6f8[];
extern u8 data_ov000_02367554[], data_ov000_0236a708[];
extern u8 data_ov000_02367568[], data_ov000_0236a720[];
extern u8 data_ov000_0236a840[], data_ov000_0236a850[], data_ov000_0236a860[], data_ov000_0236a870[];
extern u8 data_ov000_0236a880[], data_ov000_0236a890[];
extern u8 data_ov000_02367580[], data_ov000_0236a728[];
extern u8 data_ov000_0236a8a0[], data_ov000_0236a8b0[], data_ov000_0236a8c0[];
extern u8 data_ov000_0236459c[];
extern u8 data_ov000_02367598[], data_ov000_0236a710[], data_ov000_0236a910[];
extern u8 data_ov000_023675b0[], data_ov000_023675c8[], data_ov000_0236a7c0[];

void func_ov000_023265d8(void);
void func_ov000_02322f68(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void *func_ov000_02322d7c(void *a, s32 b, s32 c, s32 d, s32 e);
void PaletteAnim_Create(s32 a, void *b, u32 c, s32 d);

UnkStruct_023236c0 *func_ov000_023236c0(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023236c0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_023236c0));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023236c0));
    func_ov000_02326638(w, p, a, b);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    switch (p->unk2) {
    case 0x96:
    default:
        StageObj_Load2dBAC(w, data_ov000_02367468, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x22, data_ov000_0236a7e0);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        if (p->unk2 != 0x96)
            StageObj_SetOAMOrder(w, (u16)(w->unk184->unk2 - 0xc7));
        else
            StageObj_SetOAMOrder(w, 0x16);
        StageObj_SetAnimation(w, 0);
        break;
    case 0x97:
        StageObj_Load2dBAC(w, data_ov000_02367480, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x22, data_ov000_0236a7f0);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 1);
        StageObj_SetAnimation(w, 1);
        w->unk58 |= 0x8000000;
        break;
    case 0x98:
        StageObj_Load2dBAC(w, data_ov000_02367498, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 1, data_ov000_0236a800);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_SetAnimation(w, 2);
        w->unk58 |= 0x8000000;
        break;
    case 0x99:
        StageObj_Load2dBAC(w, data_ov000_023674b0, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 1, data_ov000_0236a810);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 1);
        StageObj_SetAnimation(w, 3);
        w->unk58 |= 0x8000000;
        break;
    case 0x9a:
        StageObj_Load2dBAC(w, data_ov000_023674c8, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 1, data_ov000_0236a820);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_SetAnimation(w, 4);
        w->unk58 |= 0x8000000;
        break;
    case 0x9b:
        StageObj_Load2dBAC(w, data_ov000_023674e0, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 1, data_ov000_0236a830);
        StageObj_AllocSpritePalette(w, 0, 0x42);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 1);
        StageObj_SetAnimation(w, 5);
        w->unk58 |= 0x8000000;
        break;
    case 0x95:
        StageObj_Load2dBAC(w, data_ov000_023674f8, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        w->unk88 = func_ov000_02322d7c(data_ov000_02367518, 0xffff, 0, 0, 0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a790);
        StageObj_AllocSpritePalette(w, 0, 0x41);
        StageObj_SetOAMOrder(w, 0x16);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f68;
        w->unk58 &= ~0x2000;
        break;
    case 0x9c:
        StageObj_Load2dBAC(w, data_ov000_02367538, 0xffff, data_ov000_0236a6f8, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x43);
        w->unk5C |= 4;
        break;
    case 0xc5:
        StageObj_Load2dBAC(w, data_ov000_02367554, 0xffff, data_ov000_0236a708, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x3c);
        StageObj_SetOAMOrder(w, 0x16);
        w->unk5C |= 4;
        break;
    case 0xc6:
    case 0xc7:
    case 0xc8:
    case 0xc9:
    case 0xca:
    case 0xcb:
    case 0xcc:
    case 0xcd:
    case 0xce:
    case 0xd6:
    case 0xd7:
        StageObj_Load2dBAC(w, data_ov000_02367568, 0, data_ov000_0236a720, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0xd6:
            StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236a840);
            StageObj_SetAnimation(w, 0);
            break;
        case 0xc7:
            w->unk5C ^= 1;
        case 0xc6:
            StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236a850);
            StageObj_SetAnimation(w, 1);
            break;
        case 0xc9:
            w->unk5C ^= 1;
        case 0xc8:
            StageObj_SetSharedSpriteVram(w, 5, data_ov000_0236a860);
            StageObj_SetAnimation(w, 2);
            break;
        case 0xcb:
            w->unk5C ^= 1;
        case 0xca:
            StageObj_SetSharedSpriteVram(w, 5, data_ov000_0236a870);
            StageObj_SetAnimation(w, 3);
            break;
        case 0xce:
            w->unk5C ^= 1;
        case 0xcc:
            StageObj_SetSharedSpriteVram(w, 5, data_ov000_0236a880);
            StageObj_SetAnimation(w, 5);
            break;
        case 0xd7:
            w->unk5C ^= 1;
        case 0xcd:
            StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236a890);
            StageObj_SetAnimation(w, 6);
            break;
        }
        StageObj_AllocSpritePalette(w, 0, 0x11);
        StageObj_SetOAMOrder(w, 0x16);
        break;
    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
        StageObj_Load2dBAC(w, data_ov000_02367580, 0, data_ov000_0236a728, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0xd1:
            w->unk5C ^= 1;
        case 0xd0:
            StageObj_SetSharedSpriteVram(w, 0x14, data_ov000_0236a8a0);
            StageObj_SetAnimation(w, 0);
            break;
        case 0xd3:
            w->unk5C ^= 1;
        case 0xd2:
            StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a8b0);
            StageObj_SetAnimation(w, 1);
            break;
        case 0xd5:
            w->unk5C ^= 1;
        case 0xd4:
            StageObj_SetSharedSpriteVram(w, 0x12, data_ov000_0236a8c0);
            StageObj_SetAnimation(w, 2);
            break;
        }
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_AllocSpritePalette(w, 0, 0x5a);
        PaletteAnim_Create(w->spr0.unk44, data_ov000_0236459c, *(u32 *)(data_ov000_0236a6f0 + 0x38), 0);
        w->unk54 |= 0x800;
        break;
    case 0xe6:
        w->unk5C ^= 1;
    case 0xe5:
        StageObj_Load2dBAC(w, data_ov000_02367598, 0, data_ov000_0236a710, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a910);
        StageObj_AllocSpritePalette(w, 0, 0x5e);
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_SetAnimation(w, 1);
        break;
    case 0xe7:
        StageObj_Load2dBAC(w, data_ov000_023675b0, 0, data_ov000_0236a710, data_ov000_02369ba0);
        w->unk88 = func_ov000_02322d7c(data_ov000_023675c8, 0xffff, 0, 4, 3);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a7c0);
        StageObj_SetAnimation(w, 0);
        StageObj_AllocSpritePalette(w, 0, 0x5e);
        StageObj_SetOAMOrder(w, 0x17);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f68;
        break;
    }
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    return w;
}
