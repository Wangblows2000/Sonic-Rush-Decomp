#include <game/Stage/stageobj.h>

UnkStruct_StageObj *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void Math_RotateVec2S16(s16 *a, s16 *b, s32 c);

void EnemyFlapperGroup_FireShot(UnkStruct_StageObj *w, s16 dx, s16 dy) {
    u16 f = 0;
    s32 type = 0xe7;
    UnkStruct_StageObj *o;

    if (w->unk5C & 1)
        f = w->unk5C & 1;
    f |= w->unk1E0->unk4;
    switch (w->unk1E0->unk2) {
    case 0xb:
        type = 0xe7;
        break;
    case 0xc:
        type = 0xeb;
        break;
    case 0x11:
        type = 0xf0;
        break;
    }
    o = MapObject_Spawn(type, w->unk10, w->unk14, f, 0, 0, 0, 0, 0);
    if (o == NULL)
        return;
    o->unk5 = w->unk5;
    Math_RotateVec2S16(&dx, &dy, w->unk5);
    if (o->unk5C & 1)
        o->unk10 -= dx << 8;
    else
        o->unk10 += dx << 8;
    o->unk14 += dy << 8;
}
