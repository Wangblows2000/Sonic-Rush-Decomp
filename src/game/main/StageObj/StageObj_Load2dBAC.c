#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 ofs;
} UnkRes_02069f1c;

typedef struct {
    u8 pad0[0x54];
    u32 flags;
    u8 pad58[0x34];
    void *owner;
    u8 pad90[4];
    u8 bgA[0x48];
    u8 bgB[0x48];
} UnkWork_02069f1c;

UnkRes_02069f1c *ObjData_Load(void *s, void *name, void *arc);
u32 ObjVram_Alloc(s32 a, u32 b);
void AnimatorSprite_Init(void *a, UnkRes_02069f1c *res, s32 b, s32 c, s32 d, s32 e, u32 f, s32 g, u32 h);

void StageObj_Load2dBAC(UnkWork_02069f1c *w, void *name, u32 pal, void *owner, void *arc) {
    UnkRes_02069f1c *res;
    u32 palA = 0;
    u32 palB = 0;

    w->flags &= ~0x100;
    w->owner = owner;
    if (arc != NULL)
        w->flags |= 0x100;
    res = ObjData_Load(w->owner, name, arc);
    if (res == NULL && arc != NULL) {
        w->flags &= ~0x100;
        res = ObjData_Load(w->owner, name, NULL);
    }
    if (pal != 0) {
        if (pal == 0xffff)
            pal = *(u16 *)((u8 *)res + res->ofs + 8);
        palA = ObjVram_Alloc(0, pal);
        palB = ObjVram_Alloc(1, pal);
    }
    AnimatorSprite_Init(w->bgA, res, 0, 0x40000, 0, 0, palA, 0, 0x05000200);
    AnimatorSprite_Init(w->bgB, res, 0, 0x40000, 1, 0, palB, 0, 0x05000600);
}
