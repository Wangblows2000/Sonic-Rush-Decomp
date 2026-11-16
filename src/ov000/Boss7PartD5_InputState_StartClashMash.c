#include <nitro/types.h>

typedef struct UnkStruct_022e9db8 UnkStruct_022e9db8;
struct UnkStruct_022e9db8 {
    u8 pad[0x43c];
    void (*func)(UnkStruct_022e9db8 *s);
    u8 pad1[4];
    s32 unk444;
};

void Boss7PartD5_InputState_ClashMash(UnkStruct_022e9db8 *s);

void Boss7PartD5_InputState_StartClashMash(UnkStruct_022e9db8 *s) {
    s->unk444 = 0;
    s->func = Boss7PartD5_InputState_ClashMash;
    s->func(s);
}
