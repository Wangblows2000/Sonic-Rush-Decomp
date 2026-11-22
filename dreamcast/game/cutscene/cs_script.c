/*
 * DemoScript (src/game/Cutscenes/DemoScript/): the story sequencer that runs
 * PlayerDemo / TransformDemo / TalkDemo in a row before a boss act, after a
 * boss, before the extra story and after the last zone, then changes scene.
 * gDemoScriptTable (0x45 entries) and gDemoScriptOpTable come from the ROM
 * (csdata.bin "ds_table"); the SceneChange target is recorded in the result.
 * DemoInput / DemoPlay (the attract demo's recorded pad input) are not part of
 * the cutscenes (CUTSCENES.md section 7).
 */
#include "cs_internal.h"

typedef struct {
    s32 n;
    s32 next;        /* CsNext of the entry's fn */
    u32 flags;
    s32 progress;
    u8 ops[4][4];    /* {u8 op, u8 arg, u16 delay} */
} DsEntry;

typedef struct DemoScript DemoScript;
struct DemoScript {
    void (*fn)(DemoScript *);
    CsNext next;     /* unk4: fn */
    u32 flags;       /* unkC */
    s16 timer;       /* unk10 */
    u16 id;          /* unk12 */
    const DsEntry *e;/* unk14 */
    s8 idx;          /* unk18 */
    u8 n;            /* unk19 */
    s32 progress;    /* unk1C */
};
static DemoScript s_ds;
static int s_ds_on;

static void DemoScript_State_Wait(DemoScript *s);
static void DemoScript_State_Delay(DemoScript *s);

static void DemoScript_ScheduleNext(DemoScript *s) {
    s->timer = 3;
    s->fn = DemoScript_State_Wait;
}
static void DemoScript_StartDelay(DemoScript *s) {
    s->timer = (s16)(s->e->ops[s->idx][2] | s->e->ops[s->idx][3] << 8);
    s->fn = DemoScript_State_Delay;
}
static void DemoScript_State_Delay(DemoScript *s) {
    s->timer--;
    if (s->timer <= 0) {
        s->idx++;
        if (s->idx >= s->n) {
            s->flags |= 0x80000000u;
            s->fn = NULL;
            return;
        }
        DemoScript_ScheduleNext(s);
    }
}
static void DemoScript_State_WaitPlayerDemo(DemoScript *s) {
    if (!PlayerDemo_IsActive())
        DemoScript_StartDelay(s);
}
static void DemoScript_State_WaitTalkDemo(DemoScript *s) {
    if (!TalkDemo_IsActive())
        DemoScript_StartDelay(s);
}
static void DemoScript_State_WaitTransformDemo(DemoScript *s) {
    if (!TransformDemo_IsActive())
        DemoScript_StartDelay(s);
}
static void DemoScript_Op_PlayerDemo(DemoScript *s) {
    gPlayerDemoId = s->e->ops[s->idx][1];
    cs_log("script %#x op %d PlayerDemo %d", s->id, s->idx, gPlayerDemoId);
    PlayerDemo_Start();
    s->fn = DemoScript_State_WaitPlayerDemo;
}
static void DemoScript_Op_TransformDemo(DemoScript *s) {
    gTransformDemoId = s->e->ops[s->idx][1];
    cs_log("script %#x op %d TransformDemo %d", s->id, s->idx, gTransformDemoId);
    TransformDemo_Start();
    s->fn = DemoScript_State_WaitTransformDemo;
}
static void DemoScript_Op_TalkDemo(DemoScript *s) {
    gTalkDemoId = s->e->ops[s->idx][1];
    cs_log("script %#x op %d TalkDemo %d", s->id, s->idx, gTalkDemoId);
    TalkDemo_Create();
    s->fn = DemoScript_State_WaitTalkDemo;
}
/* gDemoScriptOpTable */
static void (*const gDemoScriptOpTable[4])(DemoScript *) = {NULL, DemoScript_Op_PlayerDemo, DemoScript_Op_TransformDemo,
                                                             DemoScript_Op_TalkDemo};
static void DemoScript_State_Wait(DemoScript *s) {
    u8 op;
    s->timer--;
    if (s->timer > 0)
        return;
    op = s->e->ops[s->idx][0];
    if (op < 4 && gDemoScriptOpTable[op])
        gDemoScriptOpTable[op](s);
    else
        DemoScript_StartDelay(s);
}

static void DemoScript_Finish(DemoScript *w) {
    s_ds_on = 0;
    if (w->flags & 1)
        g_cs_result.progress = w->progress;   /* SaveData_Set{Sonic,Blaze}Progress + SaveData_StoreLives */
    cs_log("script %#x end -> next %d progress %d", w->id, w->next, (w->flags & 1) ? w->progress : -1);
    cs_scene_change(w->next);
}

void DemoScript_Create(s32 mode) {
    const DsEntry *t = (const DsEntry *)cs_sec("ds_table", NULL);
    u8 stage = (u8)g_cs_story.zone, id = 0;
    DemoScript *w = &s_ds;
    memset(w, 0, sizeof(*w));
    if (!t)
        return;
    switch (mode) {
    case 0:
    case 1:
        id = mode == 0 ? 0 : 0x20;
        if (g_cs_story.character == 1)
            id += 0x10;
        if (!g_cs_story.stage_new)
            id += 8;
        id += stage;
        break;
    case 3:
        id = 0x40;
        if (g_cs_story.save_flags & 0x1000)
            id++;
        break;
    case 4:
        id = 0x42;
        if (g_cs_story.character == 1)
            id++;
        break;
    case 2:
        id = 0x44;
        break;
    }
    if (id >= 0x45)
        return;
    w->id = id;
    w->e = &t[id];
    w->next = (CsNext)w->e->next;
    w->n = (u8)w->e->n;
    w->flags |= w->e->flags;
    w->progress = w->e->progress;
    s_ds_on = 1;
    cs_log("script %#x (mode %d) ops %d", id, mode, w->n);
    if (w->n == 0)
        DemoScript_Finish(w);   /* Task_Destroy, then fn() at once */
    else
        DemoScript_ScheduleNext(w);
}

/* DemoScript_StartAfterBoss: zone 8 (final) -> script mode 2 */
void DemoScript_StartAfterBoss(void) { DemoScript_Create(g_cs_story.zone == 8 ? 2 : 1); }

BOOL DemoScript_IsActive(void) { return s_ds_on; }
void DemoScript_Kill(void) { s_ds_on = 0; }

void DemoScript_RunFrame(void) {
    DemoScript *w = &s_ds;
    if (!s_ds_on)
        return;
    if (w->fn)
        w->fn(w);
    if (w->flags & 0x80000000u)
        DemoScript_Finish(w);
}
