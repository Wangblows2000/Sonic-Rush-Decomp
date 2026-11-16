#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct UnkStruct_0233d65c UnkStruct_0233d65c;
struct UnkStruct_0233d65c {
    u32 flags;
    u8 state[2];
    u8 prev[2];
    u8 pad8[0xC - 8];
    u8 unkC[2][0x28];
    u8 pad5C[0x8B4 - 0x5C];
    BOOL (*cb)(UnkStruct_0233d65c *w);
};

extern UnkStruct_0233d65c *gZoneFxWork;

void ZoneFx_Zone2_UpdateVAlarmsByCamera(UnkStruct_0233d65c *w);
void ZoneFx_Zone2_UpdateScreenStates(UnkStruct_0233d65c *w);
void ZoneFx_Zone2_QueueNormalPalettes(UnkStruct_0233d65c *w, s32 i);
void ZoneFx_Zone2_QueueTintedPalettes(UnkStruct_0233d65c *w, s32 i);
void OS_CancelVAlarm(void *p);

void ZoneFx_Zone2_TaskMain(void) {
    UnkStruct_0233d65c *w;
    BOOL force;
    u8 i;
    u8 prev;

    w = gZoneFxWork;

    if (w == NULL)
        return;
    if (w->flags & 0x80000000)
        return;
    ZoneFx_Zone2_UpdateVAlarmsByCamera(w);
    if (w->flags & 0x1000)
        return;
    w->prev[0] = w->state[0];
    w->prev[1] = w->state[1];
    if (w->cb == NULL) {
        force = FALSE;
    } else {
        force = w->cb(w);
    }
    ZoneFx_Zone2_UpdateScreenStates(w);
    for (i = 0; i < 2; i++) {
        prev = w->prev[i];
        switch (w->state[i]) {
        case 3:
            if (prev == 1)
                w->flags |= 0x40000 << i;
            if (prev != 3 || force)
                ZoneFx_Zone2_QueueNormalPalettes(w, i);
            break;
        case 0:
            if (prev == 1)
                w->flags |= 0x40000 << i;
            if (prev != 0 || force)
                ZoneFx_Zone2_QueueNormalPalettes(w, i);
            break;
        case 1:
            if (prev != 1)
                w->flags |= 0x10000 << i;
            break;
        case 2:
            if (prev == 1) {
                w->flags |= 0x40000 << i;
                OS_CancelVAlarm(w->unkC[i]);
            }
            if (prev != 2 || force)
                ZoneFx_Zone2_QueueTintedPalettes(w, i);
            break;
        }
    }
}
