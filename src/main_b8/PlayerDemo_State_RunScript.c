#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 time;
    u32 op;
    u32 arg;
} UnkCmd_02058194;

typedef struct UnkSeq_02058194 {
    void (*state)(struct UnkSeq_02058194 *);
    u32 flags;
    u8 pad8[0xc];
    void *unk14;
    u16 idx;
    u16 count;
    UnkCmd_02058194 *cmd;
    u16 time;
    s16 unk22;
} UnkSeq_02058194;

extern void (*data_0207c568[])(UnkSeq_02058194 *s, u32 arg);

void PlayerDemo_Unk_02058188(UnkSeq_02058194 *s);
UnkCmd_02058194 *PlayerDemo_Unk_0205708c(void *p);

#pragma thumb on
void PlayerDemo_State_RunScript(UnkSeq_02058194 *s) {
    for (;;) {
        UnkCmd_02058194 *c = s->cmd;
        if (s->time < c->time)
            return;
        data_0207c568[c->op](s, c->arg);
        s->idx++;
        if (s->idx >= s->count) {
            s->flags |= 4;
            s->flags |= 0x200;
            s->state = PlayerDemo_Unk_02058188;
            return;
        }
        s->cmd = PlayerDemo_Unk_0205708c(s->unk14);
        if (s->flags & 2) {
            s->unk22++;
            s->flags &= ~2;
            s->flags |= 0x100;
            return;
        }
    }
}
#pragma thumb reset
