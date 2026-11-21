#pragma opt_lifetimes off
#include <game/Stage/stageobj_alt.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0232c7b0_P;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0232c7b0_V;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x70 - 0x6C];
    void *unk70;
    u8 pad74[0x8C - 0x74];
    void **unk8C;
    u8 pad90[0x94 - 0x90];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Spr sprA[6];
    UnkStruct_Spr sprB[6];
    UnkStruct_0232c7b0_P pos[32];
    UnkStruct_0232c7b0_V vel[32];
    s16 state[32];
    u8 pad654[0x654 - 0x654];
} UnkStruct_0232c7b0;

extern UnkStruct_Task *gEffTrickSparkleTask;
extern void *gActComArchive;
extern u8 data_ov000_02368040[];
extern u8 data_ov000_0236aa18[];

void EffTrickSparkle_Destructor(void);
void EffTrickSparkle_Draw(void);
void EffTrickSparkle_State_Main(void);
void AnimatorSprite_Init(UnkStruct_Spr *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
u16 ObjDraw_AllocSpritePalette(void *a, s32 b, s32 c);

void EffTrickSparkle_Create(s32 n) {
    vu16 zero;
    u16 count;
    UnkStruct_Task *task;
    UnkStruct_0232c7b0 *w;
    u16 i;
    u16 j;

    count = 0xc;
    if (n > 2)
        return;
    count >>= n;
    if (gEffTrickSparkleTask == NULL) {
        task = Task_Create(StageObj_Main, EffTrickSparkle_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0232c7b0));
        if (task == OS_GetArenaLo(0))
            return;
        gEffTrickSparkleTask = task;
        w = task->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232c7b0));
        StageObj_Load2dBAC(w, data_ov000_02368040, 6, data_ov000_0236aa18, gActComArchive);
        for (i = 0; i < 6; i++) {
            AnimatorSprite_Init(&w->sprA[i], *w->unk8C, i, 0, 0, 0, w->spr0.unk34 + (i << 6), 0, 0x5000200);
            AnimatorSprite_Init(&w->sprB[i], *w->unk8C, i, 0, 1, 0, w->spr1.unk34 + (i << 6), 0, 0x5000600);
            if (i != 5) {
                w->sprA[i].unk44 = 1;
                w->sprB[i].unk44 = 1;
                w->sprA[i].unk2C |= 0x10;
                w->sprB[i].unk2C |= 0x10;
            } else {
                w->sprA[i].unk44 = ObjDraw_AllocSpritePalette(*w->unk8C, 5, 2);
                w->sprB[i].unk44 = w->sprA[i].unk44;
                w->sprA[i].unk2C |= 0x10;
                w->sprB[i].unk2C |= 0x10;
            }
            SpritePair_SetOAMOrder(&w->sprA[i], &w->sprB[i], 9);
        }
        w->unk70 = EffTrickSparkle_Draw;
        w->unk5C |= 0x80;
        w->unk54 |= 0x10;
        w->unk58 |= 0x2100;
        w->unk54 |= 2;
        w->unk5C |= 4;
        w->unk68 = EffTrickSparkle_State_Main;
    } else {
        UnkStruct_0232c7b0 *w2;
        w2 = gEffTrickSparkleTask->work;
        j = 0;
        for (i = 0; i < count; i++) {
            for (; j < 32; j++) {
                if (w2->state[j] == 0)
                    break;
            }
            if (j == 32)
                return;
            w2->pos[j].x = (Rand() & 0xff) << 8;
            w2->pos[j].y = 0xc000;
            w2->vel[j].x = 0x3ff - (Rand() & 0x7ff);
            w2->vel[j].y = (Rand() & 0xff) - 0x780;
            w2->state[j] = Rand() & 7;
            if (w2->state[j] == 6)
                w2->state[j] = Rand() & 3;
            if (w2->state[j] == 7)
                w2->state[j] = (Rand() & 3) + 2;
            w2->state[j]++;
        }
    }
}
