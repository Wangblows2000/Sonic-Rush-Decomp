#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 ofs;
} UnkRes_02069d38;

UnkRes_02069d38 *ObjData_Load(void *s, void *name, void *arc);
u32 ObjVram_Alloc(s32 a, u32 b);
void AnimatorSprite_Init(void *a, UnkRes_02069d38 *res, s32 b, s32 c, s32 d, s32 e, u32 f, s32 g, u32 h);

void ObjAction_Load2dBAC(void *bgA, void *bgB, void *name, u32 pal, void *s, void *arc) {
    UnkRes_02069d38 *res;
    u32 palA = 0;
    u32 palB = 0;

    res = ObjData_Load(s, name, arc);
    if (res == NULL && arc != NULL)
        res = ObjData_Load(s, name, NULL);
    if (pal != 0) {
        if (pal == 0xffff)
            pal = *(u16 *)((u8 *)res + res->ofs + 8);
        if (bgA != NULL)
            palA = ObjVram_Alloc(0, pal);
        if (bgB != NULL)
            palB = ObjVram_Alloc(1, pal);
    }
    if (bgA != NULL)
        AnimatorSprite_Init(bgA, res, 0, 0x40000, 0, 0, palA, 0, 0x05000200);
    if (bgB != NULL)
        AnimatorSprite_Init(bgB, res, 0, 0x40000, 1, 0, palB, 0, 0x05000600);
}
