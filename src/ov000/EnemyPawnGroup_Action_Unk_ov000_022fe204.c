#include <nitro/types.h>

typedef struct UnkStruct_022fe204_2 {
    u8 pad[2];
    u16 unk2;
} UnkStruct_022fe204_2;

typedef struct UnkStruct_022fe204 {
    u8 pad[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
    u8 pad2[0x1e0 - 0x6c];
    UnkStruct_022fe204_2 *unk1E0;
    u8 pad3[0x20c - 0x1e4];
    u8 unk20C[0x238 - 0x20c];
    u16 unk238;
} UnkStruct_022fe204;

void EnemyPawnGroup_Action_Unk_ov000_022fe084(UnkStruct_022fe204 *s);
void func_ov000_022faf48(UnkStruct_022fe204 *s, s32 a);
void EnemyPawnGroup_State_Unk_ov000_022fe0f8(void);
void ObjRect_SetBox2D(void *p, s32 a, s32 b, s32 c, s32 d);

void EnemyPawnGroup_Action_Unk_ov000_022fe204(UnkStruct_022fe204 *s) {
    if (s->unk1E0->unk2 == 0xd) {
        EnemyPawnGroup_Action_Unk_ov000_022fe084(s);
        return;
    }
    func_ov000_022faf48(s, 1);
    s->unk58 &= ~0x10;
    s->unk5C |= 4;
    s->unk54 &= ~0x1000;
    s->func = EnemyPawnGroup_State_Unk_ov000_022fe0f8;
    s->unk238 = 3;
    ObjRect_SetBox2D(s->unk20C, -0x78, -0x30, 0x50, -0x10);
}
