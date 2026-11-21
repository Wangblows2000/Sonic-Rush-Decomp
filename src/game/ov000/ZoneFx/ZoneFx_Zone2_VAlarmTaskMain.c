#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 unk4[4];
    u8 unk8[2];
    u8 unkA[2];
} UnkStruct_0233d324;

extern UnkStruct_0233d324 *gZoneFxWork;
extern u32 gSystemFlags;
void ZoneFx_Zone2_SetVAlarms(UnkStruct_0233d324 *w, u8 i);
void ZoneFx_Zone2_CancelVAlarms(UnkStruct_0233d324 *w, u8 i);

void ZoneFx_Zone2_VAlarmTaskMain(void) {
    UnkStruct_0233d324 *w = gZoneFxWork;
    u8 i;

    if (w == NULL)
        return;
    if (w->flags & 0x80000000)
        return;
    for (i = 0; i < 2; i++) {
        w->unkA[i] = w->unk8[i];
        if (!(gSystemFlags & 0x80000000)) {
            if (w->unk4[i] == 1)
                ZoneFx_Zone2_SetVAlarms(w, i);
            else
                ZoneFx_Zone2_CancelVAlarms(w, i);
        }
    }
}
