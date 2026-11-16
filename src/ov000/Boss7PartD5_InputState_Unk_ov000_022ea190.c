#include <nitro/types.h>

typedef struct UnkStruct_022ea190 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x18];
    s16 unk458;
} UnkStruct_022ea190;

void Boss7PartD5_InputState_Unk_ov000_022ea148(void);

s32 Boss7PartD5_InputState_Unk_ov000_022ea190(UnkStruct_022ea190 *s) {
    if (s->unk458-- == 0)
        s->func = Boss7PartD5_InputState_Unk_ov000_022ea148;
    return 0x81;
}
