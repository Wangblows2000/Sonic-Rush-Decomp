#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x324];
    u8 unk324;
} UnkStruct_02310464_P;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x68 - 0x10];
    void *unk68;
} UnkStruct_02310464;

extern UnkStruct_02310464_P *gPlayers;

void Marker_State_Unk_ov000_02310418(void);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0230ad04(UnkStruct_02310464 *w, UnkStruct_02310464_P *p);

void func_ov000_02310464(UnkStruct_02310464 *w) {
    UnkStruct_02310464_P *p = gPlayers;

    w->unk68 = Marker_State_Unk_ov000_02310418;
    w->unkC = 0x78;
    if (p->unk324 != 1) {
        if (Rand() & 1)
            Sound_PlaySeqArcEx(NULL, 0, 0x1f);
        else
            Sound_PlaySeqArcEx(NULL, 0, 0x20);
    } else {
        if (Rand() & 1)
            Sound_PlaySeqArcEx(NULL, 0, 0x28);
        else
            Sound_PlaySeqArcEx(NULL, 0, 0x29);
    }
    Sound_PlaySeqArcEx(NULL, 0, 0x31);
    func_ov000_0230ad04(w, p);
}
