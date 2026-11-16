#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    s32 offset;
} UnkRes_022dd890;

typedef struct {
    u8 pad0[0xa];
    u16 id;
} UnkHdr_022dd890;

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
} UnkSprite_022dd890;

s32 ObjVram_Alloc(s32 a, u16 id);
void AnimatorSprite_Init(void *s, void *res, s32 idx, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

#pragma thumb on
void VsMenu_Unk_ov001_022dd890(UnkSprite_022dd890 *s, UnkRes_022dd890 *res, s32 idx) {
    s32 v;
    u16 id;

    id = ((UnkHdr_022dd890 *)((u8 *)res + res->offset))->id;
    v = ObjVram_Alloc(1, id);
    AnimatorSprite_Init(s, res, idx, 0, 1, 0, v, 0, 0x5000600);
    s->flags &= ~0x60000;
}
#pragma thumb reset
