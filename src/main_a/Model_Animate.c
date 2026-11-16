#include <nitro/types.h>

void Model_AnimateMdl(void *a, void *b);
void Model_Unk_020313c8(void *a, void *b);
void AnimatorSprite_ProcessAnimation(void *a, u32 b, u32 c);

typedef struct { u32 type; u8 unk4[0x8C]; u8 unk90[0x5C]; u32 unkEC; u32 unkF0; } UnkStruct_020316f4;

void Model_Animate(UnkStruct_020316f4 *s) {
    switch (s->type) {
    case 0:
        Model_AnimateMdl(s, s);
        break;
    case 1:
        Model_Unk_020313c8(s, s);
        break;
    case 2:
        AnimatorSprite_ProcessAnimation(s->unk90, s->unkEC, s->unkF0);
        break;
    }
}
