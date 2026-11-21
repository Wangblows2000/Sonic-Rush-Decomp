#include <nitro/types.h>

typedef struct UnkStruct_022db900 {
    u8 pad[0xbc0];
    s32 unkBC0;
} UnkStruct_022db900;

s32 Boss4_GetHealthSpinScale(void);
void *_fflt(void *p);
void *_fmul(void *p, s32 a);
s32 _ffix(void *p);

void Boss4_SetSpinSpeed(UnkStruct_022db900 *s, void *p) {
    s32 v = Boss4_GetHealthSpinScale();
    s->unkBC0 = _ffix(_fmul(_fflt(p), v));
}
