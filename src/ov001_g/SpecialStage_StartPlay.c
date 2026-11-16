#include <nitro/types.h>

typedef struct UnkStruct_0230b164_br {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x20 - 8];
    u16 b0 : 1;
    u16 p1 : 5;
    u16 b6 : 2;
    u16 p8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 p11 : 1;
    u16 b12 : 1;
    u16 p13 : 3;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13b : 3;
} UnkStruct_0230b164_br;

typedef struct UnkStruct_0230b164_t {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_0230b164_t;

typedef struct UnkStruct_0230b164_o {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x48 - 0x38];
} UnkStruct_0230b164_o;

typedef struct UnkStruct_0230b164 {
    u8 pad0[0xc];
    UnkStruct_0230b164_t *taskC;
    UnkStruct_0230b164_t *task10;
    void *task14;
    s32 unk18;
    u8 pad1C[0xf8 - 0x1c];
    UnkStruct_0230b164_o objs[3];
} UnkStruct_0230b164;

typedef struct UnkStruct_0230b164_e {
    u32 a;
    u32 b;
    u32 c;
} UnkStruct_0230b164_e;

extern UnkStruct_0230b164_br gDisplayMain;
extern UnkStruct_0230b164 *gSpecialStageWork;
extern UnkStruct_0230b164_e data_ov001_023171c8[];

void Task_Destroy(void *task);
void ObjVram_Free(u32 a, u32 b);
void SpecialStagePlayer_Action_Start(s32 a);
void func_ov001_023099ec(void);
void Sound_PlaySeq(s32 a, s32 b);
void SpecialStage_State_PlayUpdate(void);
void SpecialStage_State_PlayDraw(void);

void SpecialStage_StartPlay(void) {
    UnkStruct_0230b164 *g;
    s32 i;

    g = gSpecialStageWork;
    gDisplayMain.b0 = 0;
    gDisplayMain.x = 0;
    gDisplayMain.b9 = 0;
    gDisplayMain.b10 = 0;
    gDisplayMain.b12 = 0;
    gDisplayMain.y = 0;
    gDisplayMain.b6 = 0;
    gDisplayMain.e0 = 0;
    gDisplayMain.e8 = 0;
    Task_Destroy(g->task14);
    g->task14 = NULL;
    for (i = 0; i < 3; i++)
        ObjVram_Free(data_ov001_023171c8[i].a, g->objs[i].unk34);
    SpecialStagePlayer_Action_Start(g->unk18);
    func_ov001_023099ec();
    Sound_PlaySeq(0, 0x1e);
    g->taskC->func = SpecialStage_State_PlayUpdate;
    g->task10->func = SpecialStage_State_PlayDraw;
}
