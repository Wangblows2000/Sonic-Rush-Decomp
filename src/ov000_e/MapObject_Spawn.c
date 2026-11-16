#include <ov000_c/stageobj.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} UnkStruct_ObjSpawn;

typedef UnkStruct_StageObj *(*ObjCtor)(UnkStruct_ObjSpawn *e, s32 x, s32 y, u8 i);

extern UnkStruct_ObjSpawn gMapObjectSpawnEntries[];
extern ObjCtor gMapObjectCreateTable[];

u8 MapObject_AllocSpawnEntry(void);
void MapObject_FreeSpawnEntry(UnkStruct_ObjSpawn *e);

UnkStruct_StageObj *MapObject_Spawn(s32 type, s32 x, s32 y, s16 d, s8 e, s8 f, u8 g, u8 h, u8 i) {
    UnkStruct_StageObj *o = NULL;
    s16 idx;
    UnkStruct_ObjSpawn *ent;

    idx = MapObject_AllocSpawnEntry();
    if (idx < 0x28) {
        gMapObjectSpawnEntries[idx].unk0 = 0xff;
        gMapObjectSpawnEntries[idx].unk1 = 0xff;
        gMapObjectSpawnEntries[idx].unk2 = type;
        gMapObjectSpawnEntries[idx].unk4 = d;
        gMapObjectSpawnEntries[idx].unk6 = e;
        gMapObjectSpawnEntries[idx].unk7 = f;
        gMapObjectSpawnEntries[idx].unk8 = g;
        gMapObjectSpawnEntries[idx].unk9 = h;
        gMapObjectSpawnEntries[idx].unkA = 0;
        ent = &gMapObjectSpawnEntries[idx];
        o = gMapObjectCreateTable[type](ent, x, y, i);
        if (o == NULL)
            MapObject_FreeSpawnEntry(ent);
    }
    return o;
}
