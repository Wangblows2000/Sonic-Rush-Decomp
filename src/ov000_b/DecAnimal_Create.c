#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
} UnkStruct_0231e324_Param;

typedef struct {
    u8 pad0[8];
} UnkStruct_0236a3c4;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[2];
    u32 unk8;
    u8 padC[4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x134 - 0x5E];
    UnkStruct_Coll coll0;
    u8 pad160[0x184 - 0x160];
    UnkStruct_0231e324_Param *unk184;
    u8 pad188[0x190 - 0x188];
    void *unk190;
    u32 unk194;
    u8 pad198[0x19C - 0x198];
} UnkStruct_0231e324;

extern void *gZoneActArchive;
extern void *gPlayers;
extern u16 gDecAnimalCount;
extern UnkStruct_0236a3c4 data_ov000_0236a3c4[];
extern char data_ov000_023672a0[];

void DecAnimal_Destructor(void);
void Decoration_InitFromMap(void *w, void *p, s32 a, s32 b);
void sprintf(char *buf, const char *fmt, ...);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void func_ov000_0231df98(UnkStruct_0231e324 *w);

UnkStruct_0231e324 *DecAnimal_Create(void *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_0231e324 *w;
    vu16 zero;
    char buf[32] = {0};
    s32 n;
    UnkStruct_0231e324_Param *q;

    task = Task_Create(StageObj_Main, DecAnimal_Destructor, 0, 0x500, 4, sizeof(UnkStruct_0231e324));
    if (task == OS_GetArenaLo(0))
        return NULL;
    gDecAnimalCount++;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231e324));
    Decoration_InitFromMap(w, p, a, b);
    n = w->unk184->unk2;
    if (n == 0x87)
        n = 0xf;
    sprintf(buf, data_ov000_023672a0, n + 1);
    StageObj_Load2dBAC(w, buf, 0xffff, &data_ov000_0236a3c4[n], gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 0x12);
    StageObj_SetOAMOrder(w, 0x12);
    StageObj_SetMapHitbox(w, -4, -8, 4, -1);
    ObjRect_SetBox2D(&w->coll0, -0xc, -0x14, 0xc, 6);
    w->unk58 |= 0x80;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    if (Stage_IsGravityFlipped(w->unk10, w->unk14))
        w->unk5 = 0x80;
    w->unk190 = gPlayers;
    switch (w->unk184->unk2) {
    case 0xa:
        w->unk194 |= 0x40;
        if (Rand() & 1)
            w->unk194 |= 0x10;
        break;
    case 0x87:
        if (Rand() & 1)
            w->unk194 |= 0x10;
        else
            w->unk194 |= 0x20;
        break;
    case 2:
    case 0xe:
        if (!(Rand() & 7))
            w->unk194 |= 0x10;
        else if (!(Rand() & 7))
            w->unk194 |= 0x20;
        break;
    case 4:
    case 8:
    case 0xd:
        if (Rand() & 1)
            w->unk194 |= 0x10;
        else if (Rand() & 1)
            w->unk194 |= 0x20;
        break;
    case 7:
        if (Rand() & 7)
            w->unk194 |= 0x20;
        else
            w->unk194 |= 0x10;
        break;
    case 9:
        if (Rand() & 3)
            w->unk194 |= 0x20;
        break;
    case 5:
        break;
    default:
        if (Rand() & 1) {
            if (Rand() & 1)
                w->unk194 |= 0x10;
            else
                w->unk194 |= 0x20;
        }
        if (!(Rand() & 7))
            w->unk194 |= 0x40;
        break;
    }
    q = w->unk184;
    if (q != NULL && q->unk0 == 0xff && q->unk1 == 0xff)
        w->unk30 = -0x400;
    w->unk8 = (Rand() & 0xff) + 0x168;
    if (gDecAnimalCount > 5)
        w->unk8 >>= 1;
    if (gDecAnimalCount > 6)
        w->unk8 >>= 1;
    if (gDecAnimalCount > 7)
        w->unk8 = 1;
    func_ov000_0231df98(w);
    return w;
}
