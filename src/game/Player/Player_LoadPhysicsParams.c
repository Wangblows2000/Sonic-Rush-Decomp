#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
} UnkStruct_02368b7c;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x3C - 5];
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    u8 pad44[4];
    s16 unk48;
    u8 pad4A[0x324 - 0x4A];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u8 pad330[0x356 - 0x330];
    s16 unk356;
    s16 unk358;
    s16 unk35A;
    s16 unk35C;
    s16 unk35E;
    s16 unk360;
    s16 unk362;
    s16 unk364;
    s16 unk366;
    s16 unk368;
    s16 unk36A;
    s16 unk36C;
    s16 unk36E;
    s16 unk370;
    u8 pad372[2];
    s16 unk374;
    s16 unk376;
    s16 unk378;
    s16 unk37A;
    s16 unk37C;
    s16 unk37E;
    s16 unk380;
    u8 pad382[0x3CC - 0x382];
    u8 unk3CC;
} UnkStruct_0234c644;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02368b7c gPlayerPhysicsTable[];

void Player_LoadPhysicsParams(UnkStruct_0234c644 *w) {
    w->unk356 = gPlayerPhysicsTable[w->unk324].unk0;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
        w->unk358 = gPlayerPhysicsTable[w->unk324].unk2 / 2;
    else
        w->unk358 = gPlayerPhysicsTable[w->unk324].unk2;
    w->unk35A = gPlayerPhysicsTable[w->unk324].unk4;
    w->unk35C = gPlayerPhysicsTable[w->unk324].unk6;
    w->unk35E = gPlayerPhysicsTable[w->unk324].unk8;
    w->unk360 = gPlayerPhysicsTable[w->unk324].unkA;
    w->unk362 = gPlayerPhysicsTable[w->unk324].unkC;
    w->unk364 = gPlayerPhysicsTable[w->unk324].unkE;
    w->unk366 = gPlayerPhysicsTable[w->unk324].unk10;
    w->unk368 = gPlayerPhysicsTable[w->unk324].unk12;
    w->unk36A = gPlayerPhysicsTable[w->unk324].unk14;
    w->unk36C = gPlayerPhysicsTable[w->unk324].unk16;
    w->unk36E = gPlayerPhysicsTable[w->unk324].unk18;
    w->unk370 = gPlayerPhysicsTable[w->unk324].unk26;
    w->unk374 = gPlayerPhysicsTable[w->unk324].unk1A;
    w->unk376 = gPlayerPhysicsTable[w->unk324].unk1C;
    w->unk3CC = gPlayerPhysicsTable[w->unk324].unk1E;
    w->unk378 = w->unk358 * 0.15;
    w->unk37A = w->unk358 * 0.3;
    w->unk37C = w->unk358 * 0.5;
    w->unk37E = w->unk358 * 0.7;
    w->unk380 = w->unk358 * 0.9;
    w->unk4 = 0x1c;
    w->unk3C = gPlayerPhysicsTable[w->unk324].unk20;
    w->unk3E = gPlayerPhysicsTable[w->unk324].unk22;
    w->unk40 = gPlayerPhysicsTable[w->unk324].unk28;
    w->unk42 = gPlayerPhysicsTable[w->unk324].unk2A;
    w->unk48 = gPlayerPhysicsTable[w->unk324].unk2E;
    if (w->unk32C & 0x4000000) {
        w->unk370 = (w->unk370 >> 1) + (w->unk370 >> 2);
        w->unk40 = w->unk40 >> 1;
    }
}
