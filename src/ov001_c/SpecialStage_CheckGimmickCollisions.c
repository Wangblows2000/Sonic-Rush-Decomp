#include <nitro/types.h>

typedef struct UnkStruct_0230ae4c_e {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x20 - 0x18];
    u8 unk20[0x60 - 0x20];
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
    u8 pad66[0xb4 - 0x66];
    void *unkB4;
    u8 padB8[4];
    void *unkBC;
    u8 padC0[0xdc - 0xc0];
    struct UnkStruct_0230ae4c_e *next;
} UnkStruct_0230ae4c_e;

typedef struct UnkStruct_0230ae4c_l {
    u8 pad0[0xec];
    UnkStruct_0230ae4c_e *head;
    u8 padF0[4];
    u8 sentinel[4];
} UnkStruct_0230ae4c_l;

typedef struct UnkStruct_0230ae4c {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x20 - 0x18];
    u8 unk20[0xb4 - 0x20];
    void *unkB4;
    u8 padB8[4];
    void *unkBC;
    u8 padC0[0x12c - 0xc0];
    void *unk12C;
} UnkStruct_0230ae4c;

UnkStruct_0230ae4c_l **SpecialStage_GetGimmickManager(void);
UnkStruct_0230ae4c *SpecialStage_GetPlayer(void);
BOOL SpecialStage_TestCollision(void *a, void *b, u16 c, void *d, void *e, u16 f);
void func_ov001_0230f284(UnkStruct_0230ae4c_e *e, UnkStruct_0230ae4c *p);
void SpecialStagePlayer_OnTouchGimmick(UnkStruct_0230ae4c *p, UnkStruct_0230ae4c_e *e);
void SpecialStageGimmick_OnAttacked(UnkStruct_0230ae4c_e *e, UnkStruct_0230ae4c *p);
void SpecialStagePlayer_OnAttackHit(UnkStruct_0230ae4c *p, UnkStruct_0230ae4c_e *e);
void SpecialStagePlayer_OnHurt(UnkStruct_0230ae4c *p, UnkStruct_0230ae4c_e *e);

void SpecialStage_CheckGimmickCollisions(void) {
    UnkStruct_0230ae4c_l **w = SpecialStage_GetGimmickManager();
    UnkStruct_0230ae4c *p = SpecialStage_GetPlayer();
    UnkStruct_0230ae4c_e *e;
    UnkStruct_0230ae4c_e *e2;
    UnkStruct_0230ae4c_e *e3;
    u16 ang;
    u16 ang2;
    u16 ang3;

    if (p->unk12C != NULL) {
        for (e = (*w)->head; e != (UnkStruct_0230ae4c_e *)(*w)->sentinel; e = e->next) {
            ang = 0;
            if (p->unk12C == NULL)
                break;
            if (e->unk60 & 9)
                continue;
            if (e->unkBC == NULL)
                continue;
            if ((e->unk64 & 0x2000) && !(e->unk64 & 0x4000))
                ang = e->unk16;
            if (SpecialStage_TestCollision(p->unk12C, p->unk20, p->unk16, e->unkBC, e->unk20, ang)) {
                func_ov001_0230f284(e, p);
                SpecialStagePlayer_OnTouchGimmick(p, e);
            }
        }
    }
    if (p->unkB4 != NULL) {
        for (e2 = (*w)->head; e2 != (UnkStruct_0230ae4c_e *)(*w)->sentinel; e2 = e2->next) {
            ang2 = 0;
            if (p->unkB4 == NULL)
                break;
            if (e2->unk60 & 9)
                continue;
            if (e2->unk60 & 0x100)
                continue;
            if (e2->unkBC == NULL)
                continue;
            if ((e2->unk64 & 0x2000) && !(e2->unk64 & 0x4000))
                ang2 = e2->unk16;
            if (SpecialStage_TestCollision(p->unkB4, p->unk20, p->unk16, e2->unkBC, e2->unk20, ang2)) {
                SpecialStageGimmick_OnAttacked(e2, p);
                SpecialStagePlayer_OnAttackHit(p, e2);
            }
        }
    }
    if (p->unkBC != NULL) {
        for (e3 = (*w)->head; e3 != (UnkStruct_0230ae4c_e *)(*w)->sentinel; e3 = e3->next) {
            ang3 = 0;
            if (p->unkBC == NULL)
                break;
            if (e3->unk60 & 9)
                continue;
            if (e3->unkB4 == NULL)
                continue;
            if ((e3->unk64 & 0x2000) && !(e3->unk64 & 0x4000))
                ang3 = e3->unk16;
            if (SpecialStage_TestCollision(p->unkBC, p->unk20, p->unk16, e3->unkB4, e3->unk20, ang3)) {
                SpecialStagePlayer_OnHurt(p, e3);
            }
        }
    }
}
