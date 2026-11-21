#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 6;
            u16 b6 : 2;
            u16 b8 : 1;
            u16 b9 : 1;
            u16 b10 : 1;
            u16 b11 : 1;
            u16 b12 : 4;
        } f;
    } u;
} UnkDisp_0205a654;

typedef struct {
    u8 pad0[0x5c];
    u16 flags5c;
    u8 pad5e[0x12];
    u32 unk70;
} UnkObj_0205a654;

typedef struct {
    u8 pad0[0x14];
    UnkObj_0205a654 *obj2;
    u32 unk18;
    u8 pad1c[0x458];
    void (*fn474)(void);
    u8 pad478[0x454];
    u16 unk8cc;
} UnkWork_0205a654;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[8];
    u8 zone;
    u8 act;
} UnkState_0205a654;

extern UnkState_0205a654 gGameState;
extern UnkDisp_0205a654 *gDisplayTable[];

void Hud_SetOamPriority0(void);
void Stage_InitBlend(void);
void StageTitleCard_State_FinalDelay(void);

void StageTitleCard_State_RestoreDisplay(UnkWork_0205a654 *w) {
    if (gGameState.zone != 8 || gGameState.act != 2) {
        Hud_SetOamPriority0();
        Stage_InitBlend();
    } else {
        UnkDisp_0205a654 *d = gDisplayTable[0];
        d->u.raw = 0;
        d->u.f.b6 = 1;
        d->u.f.b8 = 1;
        d->u.f.b9 = 1;
        d->u.f.b10 = 1;
        d->u.f.b11 = 1;
    }
    if (gGameState.unk10 == 1) {
        w->obj2->unk70 = w->unk18;
        w->obj2->flags5c |= 0x40;
    }
    w->unk8cc = 0;
    w->fn474 = StageTitleCard_State_FinalDelay;
}
